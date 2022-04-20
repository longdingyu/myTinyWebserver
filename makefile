CXX ?= g++

server: main.cpp config.cpp ./http/http_conn.cpp ./CGImysql/sql_connection_pool.cpp server.cpp -lpthread -lmysqlclient
#$(CXX) -o server $^ $(CXXFLAGS) -lpthread -lmysqlclient

clean:
	rm -r server