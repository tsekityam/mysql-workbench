/* 
 * Copyright (c) 2009, 2014, Oracle and/or its affiliates. All rights reserved.
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

#include "mysql_routinegroup_editor.h"
#include "GrtTemplates.h"
#include "grtdb/db_object_helpers.h"

using namespace System;
using namespace System::Collections::Generic;

namespace MySQL {
namespace Grt {
namespace Db {

public ref class MySQLRoutineGroupEditorWrapper : public RoutineGroupEditorWrapper
{
protected:
  MySQLRoutineGroupEditorWrapper(::MySQLRoutineGroupEditorBE *inn);

public:
  MySQLRoutineGroupEditorWrapper(MySQL::Grt::GrtManager^ grtm, MySQL::Grt::GrtValue^ arglist);

  ::MySQLRoutineGroupEditorBE *get_unmanaged_object();
  void load_routines_sql();
  void commit_changes();
};

} // namespace Db
} // namespace Grt
} // namespace MySQL