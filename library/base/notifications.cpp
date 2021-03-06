/* 
 * Copyright (c) 2011, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "base/notifications.h"
#include "base/log.h"
#include <stdexcept>

DEFAULT_LOG_DOMAIN(DOMAIN_BASE);

using namespace base;

static NotificationCenter *nc = 0;

void NotificationCenter::set_instance(NotificationCenter *center)
{
  std::map<std::string, NotificationHelp> help;
  
  if (nc) 
  {
    help = nc->_notification_help;
    delete nc;
  }
  nc = center;
  nc->_notification_help = help;
}


NotificationCenter *NotificationCenter::get()
{
  if (!nc)
    nc = new NotificationCenter();
  return nc;
}

//--------------------------------------------------------------------------------------------------

Observer::~Observer()
{
  NotificationCenter *nc = NotificationCenter::get();
  if (nc->is_registered(this))
    log_error("Notifications: Observer %p was deleted while still listening for notifications.\n", this);
}

//--------------------------------------------------------------------------------------------------

NotificationCenter::~NotificationCenter()
{
  if (_observers.size() > 0)
  {
    log_error("Notifications: The following observers are not unregistered:\n");

    for (std::list<ObserverEntry>::iterator next, iter = _observers.begin(); iter != _observers.end(); ++iter)
      log_error("\tObserver %p, for message: %s\n", iter->observer, iter->observed_notification.c_str());
  }
}

//--------------------------------------------------------------------------------------------------

void NotificationCenter::register_notification(const std::string &name,
                                               const std::string &context,
                                               const std::string &general_info,
                                               const std::string &sender_info,
                                               const std::string &info_info)
{
  NotificationHelp help;
  help.context = context;
  help.summary = general_info;
  help.sender = sender_info;
  help.info = info_info;
  _notification_help[name] = help;
}


void NotificationCenter::add_observer(Observer *observer, const std::string &name)
{
  ObserverEntry entry;
  entry.observer = observer;
  entry.observed_notification = name;
  _observers.push_back(entry);
}

/*
void NotificationCenter::add_observer(Observer *observer, boost::function<void (const std::string &, void*, NotificationInfo &)> &callback, const std::string &name)
{
  ObserverEntry entry;
  entry.observer = observer;
  entry.callback = callback;
  entry.observed_notification = name;
  _observers.push_back(entry);
}*/


bool NotificationCenter::remove_observer(Observer *observer, const std::string &name)
{
  bool found = false;
  for (std::list<ObserverEntry>::iterator next, iter = _observers.begin(); iter != _observers.end();)
  {
    next = iter;
    ++next;
    if (iter->observer == observer && (name.empty() || name == iter->observed_notification))
    {
      found = true;
      _observers.erase(iter);
    }
    iter = next;
  }
  if (!found)
    log_debug("remove_observer: %p for %s failed to remove any observers\n", observer, name.c_str());
  return found;
}

//--------------------------------------------------------------------------------------------------

bool NotificationCenter::is_registered(Observer *observer)
{
  for (std::list<ObserverEntry>::iterator next, iter = _observers.begin(); iter != _observers.end(); ++iter)
  {
    if (iter->observer == observer)
      return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

void NotificationCenter::send(const std::string &name, void *sender, NotificationInfo &info)
{
  if (name.substr(0, 2) != "GN")
    throw std::invalid_argument("Attempt to send notification with a name that doesn't start with GN\n");
  
  if (_notification_help.find(name) == _notification_help.end())
    log_info("Notification %s is not registered\n", name.c_str());
  
  // act on a copy of the observer list, because one of them could remove stuff from the list
  std::list<ObserverEntry> copy(_observers);
  for (std::list<ObserverEntry>::iterator iter = copy.begin(); iter != copy.end(); ++iter)
  {
    if (iter->observed_notification.empty() || iter->observed_notification == name)
    {
      //if (iter->callback)
      //  iter->callback(name, sender, info);
      //else
      iter->observer->handle_notification(name, sender, info);
    }
  }
}


void NotificationCenter::send(const std::string &name, void *sender)
{
  NotificationInfo info;
  send(name, sender, info);
}
