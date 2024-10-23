# tasktracker
Task tracker CLI

## Комманды
```shell
./bin/task-cli list
./bin/task-cli add "task 1" && ./bin/task-cli list
./bin/task-cli update 1 "Updated task 1" && ./bin/task-cli list
./bin/task-cli mark-progress 1 && ./bin/task-cli list
./bin/task-cli mark-in-progress 1 && ./bin/task-cli list
./bin/task-cli mark-done 1 && ./bin/task-cli list
./bin/task-cli mark-is-done 1 && ./bin/task-cli list
./bin/task-cli mark-todo 1 && ./bin/task-cli list
./bin/task-cli delete 1 && ./bin/task-cli list
```


## Сборка и запуск
- Linux (MacOS)
```shell
cmake --preset="dev-linux"
cmake --build build
./bin/task-cli list
```
- Windows
```shell
cmake --preset="dev-win"
cmake --build build
./bin/task-cli.exe list
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