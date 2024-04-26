#pragma once

#include "api_framework.h"
#include "connection_pool.h"
#include "connection_object.h"
#include "connection_work.h"
#include "connection_table.h"

namespace DP
{
	const int POSTGRESQL_UPDATE_VALUES = 134;

	namespace POSTGRESQL
	{

		template<typename ITEM>
		class UpdateValues : public ConnectionWork
		{
		public:
			UpdateValues(
				TConnectionPool connectionPool,
				size_t requestId,
				std::shared_ptr<ConnectionTable<ITEM>> table,
				TComponent component,
				std::shared_ptr<Composite<bool>> columns,
				std::string where
			) : ConnectionWork(
				connectionPool,
				requestId,
				insertValueScript(table, component, columns, where)
			) { }

			virtual const int pid() override { return POSTGRESQL_UPDATE_VALUES; };

			std::string insertValueScript(
				std::shared_ptr<ConnectionTable<ITEM>> table,
				TComponent component,
				std::shared_ptr<Composite<bool>> columns,
				std::string where)
			{
				std::string arguments = "";
				for (size_t i = 0; i < columns->size(); i++)
				{
					if (columns->get(i))
					{
						if (arguments == "")
						{
							arguments = table->column(i) + " = " + component->toString(i);
						}
						else {
							arguments = arguments + ", " + table->column(i) + " = " + component->toString(i);
						}

					}
				}
				return "UPDATE " + table->name() + " SET \n " + arguments + "\n WHERE " + where + "; \n";
			}
		};
	}
}