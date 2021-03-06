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

#pragma once

namespace MySQL {
  namespace Forms {

    public class FindPanelWrapper : public ViewWrapper
    {
    protected:
      FindPanelWrapper(mforms::FindPanel *backend);

      static bool create(mforms::FindPanel *backend);
      static size_t perform_action(mforms::FindPanel *backend, mforms::FindPanelAction action);
      static void focus(mforms::FindPanel *backend);
      static void enable_replace(mforms::FindPanel *backend, bool flag);
    public:
      static void init();
    };

  };
};
