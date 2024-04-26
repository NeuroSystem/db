#pragma once
#include <utility>
#include "api_framework.h"
#include "connection_pool.h"
#include "connection_object.h"

namespace DP
{
	const int POSTGRESQL_CONNECTION_WORK = 132;

	namespace POSTGRESQL
	{

		class ConnectionWork : public ConnectionObject
		{
		public:
			ConnectionWork(TConnectionPool connectionPool, size_t requestId, std::string sqlRequest) : ConnectionObject(connectionPool, requestId)
			{
				currentSql = sqlRequest;
			}

			virtual const int pid() override { return POSTGRESQL_CONNECTION_WORK; };

			virtual void onResult(pqxx::result response)
			{
				int responseSize = response.size();
				int* columnTypes{ new int[response.columns()] };
				int* tableColumn{ new int[response.columns()] };
				std::string* columnNames{ new std::string[response.columns()] };

				for (pqxx::row_size_type column_index = 0; column_index < response.columns(); ++column_index)
				{
					columnNames[column_index] = getColumnName(response, column_index);
					columnTypes[column_index] = getColumnType(response, column_index);
					tableColumn[column_index] = getTableColumn(response, column_index);

					std::cout << "index: " << column_index << " name: " << columnNames[column_index] << " type: " << columnTypes[column_index] << " col: "<< tableColumn[column_index] << '\n';
				}

				std::cout << "ID: " << id << " SQL: " << currentSql << " COUNT: " << response.size() << std::endl;

				for (pqxx::result::const_iterator row = response.begin(); row != response.end(); ++row)
				{

					std::cout << "ID: " << row[0].as<int>() << std::endl;
					auto arr = row[1].as_array();
					std::pair<pqxx::array_parser::juncture, std::string> elem;
					do
					{
						elem = arr.get_next();
						if ((elem.first == pqxx::array_parser::juncture::string_value))
						{
							std::cout << "value: " << stoul(elem.second) << std::endl;
						}
					} while (elem.first != pqxx::array_parser::juncture::done);
				}


				
				delete[] columnTypes;
				delete[] columnNames;
			}

			virtual void executeQuery(TPostgres connection) override
			{
				std::cout << "EXECUTE" << std::endl;
				pqxx::work W(reinterpret_cast<PostgreSQL&>(*connection.get()));
				onResult(W.exec(currentSql));
				W.commit();
			}
		};
	}
}