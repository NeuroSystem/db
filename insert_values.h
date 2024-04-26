#pragma once

#include "api_framework.h"
#include "connection_pool.h"
#include "connection_object.h"
#include "connection_work.h"
#include "connection_table.h"

namespace DP
{
	const int POSTGRESQL_INSERT_VALUES = 133;

	namespace POSTGRESQL
	{

		template<typename ITEM>
		class InsertValues : public ConnectionWork
		{
		private:
			std::string getAllValues(std::shared_ptr<Composite<ITEM>> composite)
			{
				std::string values = "";
				int iCount = composite->size() - 1;
				for (size_t i = 0; i < iCount; i++)
				{
					values = values + " (" + composite->get(i)->toString() + "), \n";
				}
				values = values + " (" + composite->get(iCount)->toString() + ") ";
				return values;
			}

		public:
			InsertValues(TConnectionPool connectionPool, size_t requestId, std::shared_ptr<ConnectionTable<ITEM>> table, TComponent component) :
				ConnectionWork(
					connectionPool,
					requestId,
					insertValueScript(table, component)
				) { }

			InsertValues(TConnectionPool connectionPool, size_t requestId, std::shared_ptr<ConnectionTable<ITEM>> table, std::shared_ptr<DP::Composite<ITEM>> composite, bool value) :
				ConnectionWork(
					connectionPool,
					requestId,
					insertValuesScript(table, composite)
				) { }

			virtual const int pid() override { return POSTGRESQL_INSERT_VALUES; };

			std::string insertValueScript(std::shared_ptr<ConnectionTable<ITEM>> table, TComponent component)
			{
				return "INSERT INTO " + table->name() + "(\n " + table->allColumns() + ") \n VALUES (" + component->toString() + "); \n";
			}

			std::string insertValuesScript(std::shared_ptr<ConnectionTable<ITEM>> table, std::shared_ptr<DP::Composite<ITEM>> composite)
			{
				return "INSERT INTO " + table->name() + "(\n " + table->allColumns() + ") \n VALUES " + getAllValues(composite) + "; \n";
			}
		};
	}
}