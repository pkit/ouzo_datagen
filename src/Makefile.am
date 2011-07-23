bin_PROGRAMS=ouzo_datagen test_main
ouzo_datagen_SOURCES=ouzo_data_gen.cpp schema/epaper.pb.cc \
	 conversion/DremelReader.cpp conversion/DremelWriter.cpp \
	 conversion/SqliteWriter.cpp builder/DocumentsBuilder.cpp \
	 conversion/sqlite/Database.cpp conversion/sqlite/Query.cpp \
	 conversion/sqlite/sqlite3.c conversion/ProtobufWriter.cpp conversion/ProtobufReader.cpp

test_main_SOURCES=test/test_main.cc test/gtest/gtest-all.cc \
	conversion/DremelReader.cpp conversion/DremelWriter.cpp \
	builder/DocumentsBuilder.cpp conversion/ProtobufWriter.cpp \
	conversion/ProtobufReader.cpp schema/epaper.pb.cc
