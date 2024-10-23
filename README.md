# tasktracker
Task tracker CLI

Sample solution for the [task-tracker](https://roadmap.sh/projects/task-tracker) challenge for [roadmap.sh](https://roadmap.sh)
- Linux
```shell
git clone git@github.com:babenow/tasktracker.git
cd tasktracker && cmake --preset="dev-linux" && cmake --build build
```
- Windows
```shell
git clone git@github.com:babenow/tasktracker.git
cd tasktracker && cmake --preset="dev-win" && cmake --build build
```
## Комманды
```shell
./bin/task-cli add "Test task" && ./bin/task-cli list
./bin/task-cli update 1 "Updated task 1" && ./bin/task-cli list
./bin/task-cli mark-progress 1 && ./bin/task-cli list
./bin/task-cli mark-in-progress 1 && ./bin/task-cli list
./bin/task-cli mark-done 1 && ./bin/task-cli list
./bin/task-cli mark-is-done 1 && ./bin/task-cli list
./bin/task-cli mark-todo 1 && ./bin/task-cli list
./bin/task-cli delete 1 && ./bin/task-cli list
```


## Добавление задачи
```c++
auto tasks_path = std::filesystem::path(argv[0]).parent_path().append("tasks.json");
auto tasks_list = std::make_shared<TaskList>(tasks_path.c_str());
tasks_list->Add({
    descripition: "Bye bread",
});
task_list->Save();
```