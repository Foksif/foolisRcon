
# foolisRcon

`foolisRcon` — простой CLI RCON-клиент на C++ для подключения к серверам Minecraft (и другим серверам с поддержкой RCON). Поддерживает профили серверов в формате JSON и автоматически создаёт конфигурационный файл при первом запуске.

---

## Возможности

- 🔐 Поддержка RCON-аутентификации
- ⚙️ Конфигурационные профили (`~/.config/foolisRcon/servers.json`)
- 🛠️ Автоматическое создание конфигурационного файла
- 📄 Поддержка нескольких серверов
- 🧼 Чистый и лёгкий код на C++17

---

## Установка

### Требования

- Linux
- CMake 3.14+
- GCC или Clang с поддержкой C++17
- [nlohmann/json](https://github.com/nlohmann/json) (установить через `pacman -S nlohmann-json`)

### Сборка

```bash
git clone https://github.com/Foksif/foolisRcon.git
cd foolisRcon
mkdir build && cd build
cmake ..
make
./foolisRcon
```

---

## Конфигурация

При первом запуске создаётся файл:

```
~/.config/foolisRcon/servers.json
```

Пример содержимого:

```json
{
  "local": {
    "host": "127.0.0.1",
    "port": 25575,
    "password": "changeme"
  },
  "prod": {
    "host": "mc.elysium-smp.fun",
    "port": 25575,
    "password": "supersecret"
  }
}
```

---

## Использование

```bash
./foolisRcon
```

Затем вводите имя профиля:

```
Enter profile name: local
> time set day
> say Привет от foolisRcon!
> exit
```

---

## Структура проекта

```
foolisRcon/
├── main.cpp
├── RconClient.hpp / .cpp
├── utils/
│   └── Network.hpp / .cpp
├── CMakeLists.txt
└── README.md
```

---

## Заметки

Убедитесь, что в настройках вашего Minecraft-сервера включён RCON (`server.properties`):

- `enable-rcon=true`
- Заданы `rcon.password` и `rcon.port`

---

## Как помочь проекту

Будем рады любым вкладом! Пожалуйста, придерживайтесь правил:

- Форкните репозиторий и создайте ветку от `main`.
- Пишите понятные и описательные сообщения коммитов.
- Создавайте pull request с подробным описанием изменений.

---

## Лицензия

MIT — свободное использование, изменение и распространение.

---

