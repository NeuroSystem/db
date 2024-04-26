#pragma once
#include "db_framework.h"
#include "connection_pool.h"
#include "connection_object.h"
#include "connection_table.h"
#include "connection_work.h"
#include "select_table.h"
#include "create_table.h"
#include "insert_values.h"
#include "update_values.h"

#include "database_functions.h"

#include "console_commands.h"

namespace DP
{

	namespace API
	{
		const int API_EMPTY = 0;
		const int API_START = 1;
		const int API_STOP = 2;
		const int API_OBJECT_TYPE = 3;
		const int API_INT = 4;
		const int API_DOUBLE = 5;
		const int API_STRING = 6;
		const int API_VECTOR = 7;

		const int API_END = 100;


	}

}

