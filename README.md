# Description
Утилита скачивает, парсит и сохраняет фиды формата Yandex Realty Language.

# Install:
`$ sudo dpkg -i yrl-<version>.deb`

# Uninstall:
`$ sudo dpkg -r yrl`

# Usage:
`$ /opt/yrl/bin/yrl_feed <config filename>`

# Config file
Пример конфига устанавливается в `/opt/yrl/etc/config_example`.
Все строки, начинающиеся с символа `#`, игнорируются.

Формат:
 - **host**: mongo db host;
 - **port**: mongo db port;
 - **storage**: path to storage
 - **only_sales**: флаг, требующий обрабатывать только офферы с типом "продажа";
 - **only_flats**: флаг, требующий обрабатывать только офферы с категорией "квартира".

# Build
Install following packages for building project:
- **libxml2-dev**
- **libsigc++-2.0-dev**
- **libglibmm-2.4-dev**
- **libxml++2.6-dev**
- **libgtest-dev**
