#pragma once
#include "design_pattern.h"

namespace DP
{
	const int POSTGRESQL_CONNECTION_TABLE = 130;

	namespace POSTGRESQL
	{

		template<typename ITEM>
		class ConnectionTable : public DP::Component
		{
		public:
			std::string allColumns()
			{
				std::string columns = "";
				int iCount = columnsCount() - 1;
				for (size_t i = 0; i < iCount; i++)
				{
					columns = columns + column(i) + ", ";
				}
				columns = columns + column(iCount);
				return columns;
			}
			pqxx::field column(pqxx::result::const_iterator row, int index) { return row[index]; }
			virtual std::string column(int index) { return ""; }
			virtual std::string columnType(int index) { return ""; }
			virtual std::string columns() { return ""; }
			virtual int columnsCount() { return 0; }
			virtual ITEM create(pqxx::result::const_iterator row) { return 0; }
			virtual std::string name() { return ""; }
			virtual const int pid() override { return POSTGRESQL_CONNECTION_TABLE; };
		};

		template<typename ITEM>
		using TConnectionTable = std::shared_ptr<ConnectionTable<ITEM>>;
	}
}