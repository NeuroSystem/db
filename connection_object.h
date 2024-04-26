#pragma once
#include "api_framework.h"
#include "connection_pool.h"

namespace DP
{
	const int POSTGRESQL_CONNECTION_OBJECT = 129;

	namespace POSTGRESQL
	{

		class ConnectionObject : public Object
		{
		protected:
			TConnectionPool pool;
			std::string currentSql = "";
			size_t id;

			char const* getColumnName(pqxx::result response, pqxx::row_size_type column_index) { return response.column_name(column_index); }
			pqxx::oid getColumnType(pqxx::result response, pqxx::row_size_type column_index) { return response.column_type(column_index); }
			pqxx::row_size_type getTableColumn(pqxx::result response, pqxx::row_size_type column_index) { return response.table_column(column_index); }

			const int getInt(pqxx::field field) { return field.as<int>(0); }
			const bool getBool(pqxx::field field) { return field.as<bool>(false); }
			const float getFloat(pqxx::field field) { return field.as<float>(0.0); }
			const time_t getTimeT(pqxx::field field) { return field.as<time_t>(0); }
			const double getDouble(pqxx::field field) { return field.as<double>(0.0); }
			const unsigned int getUInt(pqxx::field field) { return field.as<unsigned int>(0); }
			const std::string getString(pqxx::field field) { return field.as<std::string>(""); }

		public:
			ConnectionObject(TConnectionPool connectionPool, size_t requestId) : pool(connectionPool), id(requestId) {}

			virtual const int pid() override { return POSTGRESQL_CONNECTION_OBJECT; };

			void execute()
			{
				TPostgres connection = pool->lock();
				try {
					executeQuery(connection);
				}
				catch (const std::exception & e) {
					pool->onError(e, currentSql, id, connection->backendpid(), pid());
				}
				pool->unlock(connection);
			}

			virtual void executeQuery(TPostgres connection) { }
		};
	}
}