#pragma once

#include "api_framework.h"

namespace DP
{
	const int POSTGRESQL_CONNECTION_POOL = 128;

	namespace POSTGRESQL
	{

		class ConnectionPool : public ObjectPool<TPostgres>
		{
		public:
			ConnectionPool() : ObjectPool() {}

			void init(std::string connectionString, int poolSize) 
			{
				std::lock_guard<std::mutex> locker_(m_mutex);
				for (auto i = 0; i < poolSize; i++) {
					m_pool.emplace(std::make_shared<PostgreSQL>(connectionString.c_str()));
				}
			}

			virtual const int pid() override { return POSTGRESQL_CONNECTION_POOL; };

			virtual void unlock(TPostgres connection) override
			{
				std::unique_lock<std::mutex> lock_(m_mutex);

				// push a new connection into a pool
				m_pool.push(connection);

				// unlock mutex
				lock_.unlock();

				// notify one of thread that is waiting
				m_condition.notify_one();
			}

			virtual TPostgres lock() override
			{
				std::unique_lock<std::mutex> lock_(m_mutex);

				// if pool is empty, then wait until it notifies back
				while (m_pool.empty()) {
					m_condition.wait(lock_);
				}

				// get new connection in queue
				auto connection = m_pool.front();
				// immediately pop as we will use it now
				m_pool.pop();

				return connection;
			}

			void onError(const std::exception& e, std::string sql, size_t id, size_t backendId, int pid)
			{
				std::cout << "ERROR PostgreSQL backend_id: " << backendId << " sqlId: " << id << " objectId: " << pid << "\n SQL:" << sql << std::endl;
				std::cerr << e.what() << std::endl;
			}

		private:
			std::mutex m_mutex;
			std::condition_variable m_condition;
			std::queue<TPostgres> m_pool;
		};

		using TConnectionPool = std::shared_ptr<ConnectionPool>;
	}
}