#pragma once
#include <iostream>
#include <thread>

#include "api_framework.h"
#include "connection_pool.h"
#include "connection_object.h"
#include "connection_table.h"
#include "connection_work.h"
#include "select_table.h"
#include "create_table.h"
#include "insert_values.h"
#include "update_values.h"
#include "database_functions.h"

namespace DP
{

	namespace POSTGRESQL
	{

		void sqlWorker(TConnectionPool connectionPool, size_t poolId, std::string sqlRequest)
		{
			ConnectionWork task(connectionPool, poolId, sqlRequest);
			task.execute();
		}



		void rawSelect(std::shared_ptr<ConnectionPool> pool, size_t id)
		{
			auto conn = pool->lock();

			try {
				// create a transaction from a connection
				pqxx::work W(reinterpret_cast<pqxx::connection&>(*conn.get()));
				pqxx::result response = W.exec("SELECT * FROM public.\"Users\";");
				for (size_t i = 0; i < response.size(); i++)
				{
					std::cout << "id: " << response[i][0] << " login: " << response[i][1] << " password: " << response[i][2] << " email: " << response[i][3] << std::endl;
				}
				std::cout << "ID: " << id << std::endl;
				W.commit();

				// free connection when things done
				pool->unlock(conn);
			}
			catch (const std::exception & e) {
				std::cerr << e.what() << std::endl;
				// free connection when error kicks in
				pool->unlock(conn);
			}
		}

		void createDoubleArray(std::shared_ptr<ConnectionPool> pool, size_t id)
		{
			auto conn = pool->lock();

			try {
				// create a transaction from a connection
				pqxx::work W(reinterpret_cast<pqxx::connection&>(*conn.get()));
				pqxx::result response = W.exec("SELECT * FROM public.\"Users\";");
				for (size_t i = 0; i < response.size(); i++)
				{
					std::cout << "id: " << response[i][0] << " login: " << response[i][1] << " password: " << response[i][2] << " email: " << response[i][3] << std::endl;
				}
				std::cout << "ID: " << id << std::endl;
				W.commit();

				// free connection when things done
				pool->unlock(conn);
			}
			catch (const std::exception & e) {
				std::cerr << e.what() << std::endl;
				// free connection when error kicks in
				pool->unlock(conn);
			}
		}

	}

}
