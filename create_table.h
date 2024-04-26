#pragma once

#include "api_framework.h"
#include "connection_pool.h"
#include "connection_object.h"
#include "connection_work.h"
#include "connection_table.h"

namespace DP
{
	const int POSTGRESQL_CREATE_TABLE = 132;

	namespace POSTGRESQL
	{

		template<typename ITEM>
		class CreateTable : public ConnectionWork
		{
		public:
			CreateTable(TConnectionPool connectionPool, size_t requestId, std::shared_ptr<ConnectionTable<ITEM>> table) :
				ConnectionWork(connectionPool, requestId, createTableScript(table)) { }

			virtual const int pid() override { return POSTGRESQL_CREATE_TABLE; };

			std::string createTableScript(std::shared_ptr<ConnectionTable<ITEM>> table)
			{
				std::string list = "";
				int iCount = table->columnsCount() - 1;
				for (size_t i = 0; i < iCount; i++)
				{
					list = list + "    " + table->column(i) + " " + table->columnType(i) + ",\n";
				}
				list = list + "    " + table->column(iCount) + " " + table->columnType(iCount) + "\n";
				return "CREATE TABLE " + table->name() + "(\n" + list + ");\n";
			}
		};
	}
}