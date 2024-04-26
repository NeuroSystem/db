#pragma once
#include "api_framework.h"
#include "connection_pool.h"
#include "connection_object.h"
#include "connection_table.h"

namespace DP
{
	const int POSTGRESQL_SELECT_TABLE = 131;

	namespace POSTGRESQL
	{

		template<typename ITEM>
		class SelectTable : public ConnectionObject
		{
		public:
			std::shared_ptr<DP::Composite<ITEM>> composite;
			std::shared_ptr<ConnectionTable<ITEM>> table;

			SelectTable(
				TConnectionPool connectionPool,
				size_t requestId,
				std::string script,
				std::shared_ptr<ConnectionTable<ITEM>> source,
				std::shared_ptr<DP::Composite<ITEM>> result
			) : ConnectionObject(connectionPool, requestId)
			{
				currentSql = script;
				composite = result;
				table = source;
			}

			virtual const int pid() override { return POSTGRESQL_SELECT_TABLE; };

			virtual void onSelect(pqxx::result response) {
				std::cout << "ID: " << id << " SQL: " << currentSql << " COUNT: " << response.size() << std::endl;
				for (pqxx::result::const_iterator row = response.begin(); row != response.end(); ++row) {
					composite->add(table->create(row));
				}
			}

			virtual void executeQuery(TPostgres connection) override
			{
				pqxx::nontransaction N(reinterpret_cast<PostgreSQL&>(*connection.get()));
				onSelect(N.exec(currentSql));
			}
		};
	}
}