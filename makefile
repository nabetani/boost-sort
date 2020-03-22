graph.pdf : list.csv graph.py
	TK_SILENCE_DEPRECATION=1 python graph.py 

list.csv : CMakeLists.txt clang/CMakeLists.txt gcc9/CMakeLists.txt makelist.rb main.cpp
	ruby makelist.rb
