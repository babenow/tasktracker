# tasktracker
Task tracker CLI


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