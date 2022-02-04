cd cpp;
g++ --std=c++17 main.cpp utils/graph_utils.cpp -pthread;
cd ..;
python3 manage.py runserver;