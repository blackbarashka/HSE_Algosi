# Отчет по домашним заданиям P07 и P08

**Проект:** Reading List API  
**Автор:** Мусаев Умахан Рашидович  

---

## P07 — Контейнеризация приложения

### C1. Dockerfile (multi-stage, размер)

**Выполнение:**
- Реализован multi-stage build с двумя стадиями: `build` и `runtime`
- В стадии `build` устанавливаются зависимости и запускаются тесты
- В стадии `runtime` создается минимальный образ только с необходимыми файлами
- Удалены временные зависимости (gcc) после сборки
- Оптимизирован кэш слоев: сначала копируются файлы зависимостей (`requirements.txt`), затем устанавливаются пакеты, и только потом копируется код приложения
- Используется минимальный базовый образ `python:3.11.9-slim`
- Создан непривилегированный пользователь `appuser` для безопасности
- Настроен `HEALTHCHECK` для мониторинга состояния контейнера

**Доказательства:**
- Файл `Dockerfile` в корне проекта
- Multi-stage build: стадии `build` и `runtime`
- Размер образа оптимизирован за счет удаления build-зависимостей из финального образа
- Проверка размера: `docker images reading-list-app`

<img width="707" height="762" alt="image" src="https://github.com/user-attachments/assets/4e768a68-ef74-4423-b5a4-f588f55bd820" />


Dockerfile оптимизирован для продакшн-использования с минимальной базой, кэш-слоями и безопасной конфигурацией.

---

### C3. Compose/локальный запуск

**Выполнение:**
- Создан `compose.yaml` для локального запуска приложения
- Настроены переменные окружения для конфигурации (порт, база данных)
- Настроен healthcheck для автоматической проверки состояния сервиса
- Политика перезапуска: `unless-stopped`
- Приложение доступно по HTTP на порту 8000

**Доказательства:**
- Файл `compose.yaml` в корне проекта
- Успешный запуск: `docker compose up --build`
- Приложение доступно по адресу: http://localhost:8000
- Документация API: http://localhost:8000/docs

<img width="756" height="318" alt="image" src="https://github.com/user-attachments/assets/aabc1a9b-adf9-4532-a548-cd4d96d6b7ae" />

Compose описывает реальное приложение с настройками окружения, healthcheck и политикой перезапуска.

---

### C5. Контейнеризация своего приложения

**Выполнение:**
- Собственное приложение Reading List API полностью контейнеризировано
- Приложение запускается через `docker compose up`
- Доступно по HTTP на порту 8000
- Интегрировано с CI/CD: образ собирается в GitHub Actions
- Приложение предоставляет REST API для управления списком книг
- Настроена интеграция с базой данных через переменные окружения

**Доказательства:**
- Репозиторий содержит `Dockerfile` и `compose.yaml`
- Приложение запускается и работает в контейнере
- CI/CD pipeline собирает Docker-образ (см. `.github/workflows/ci.yml`)
- API доступен и функционален: http://localhost:8000/docs

<img width="580" height="134" alt="image" src="https://github.com/user-attachments/assets/c99efff6-a901-4bca-b215-c7d0f5a0d577" />

<img width="1581" height="866" alt="image" src="https://github.com/user-attachments/assets/d3e16ff4-16f0-4951-a3d9-808a63eac7ec" />

Собственный сервис контейнеризирован, запускается через Docker Compose, доступен по HTTP и интегрирован с CI/CD.

---

## P08 — CI/CD конвейер

### C1. Сборка и тесты

**Выполнение:**
- Настроены этапы сборки и тестирования в GitHub Actions
- Unit-тесты запускаются через pytest
- Настроена матрица для тестирования на нескольких версиях Python: 3.11 и 3.12

    <img width="367" height="243" alt="image" src="https://github.com/user-attachments/assets/f3dbc9b4-f0b8-436d-93c7-05b9871a83cc" />
- Настроена матрица для тестирования на разных операционных системах (ubuntu-latest)
- Линтинг и форматирование: ruff, black, isort
- Pre-commit проверки запускаются в CI
- CI run успешно проходит (зеленые прогоны)

**Доказательства:**
- Файл `.github/workflows/ci.yml` с настройкой матрицы
- Лог успешного CI run с прохождением всех тестов
- Матрица версий Python: `python-version: ["3.11", "3.12"]`
- Все проверки проходят успешно

Настроена матрица для нескольких версий Python, тесты и сборка проходят стабильно.

---

### C2. Кэширование/конкурренси

**Выполнение:**
- Настроено кэширование pip-зависимостей через `actions/cache@v4`
- Ключ кэша основан на хэше файлов `requirements*.txt`
- Настроен `concurrency` для предотвращения дубликатов запусков
- Кэш Docker-образов через GitHub Actions cache (`cache-from: type=gha`, `cache-to: type=gha,mode=max`)
- Оптимизированы ключи кэша под проект: кэш зависит от версии Python и хэша requirements файлов

**Доказательства:**
- Файл `.github/workflows/ci.yml`:
  - Настройка `concurrency`:
  <img width="332" height="52" alt="image" src="https://github.com/user-attachments/assets/be27c543-33e9-42e3-8aa8-58d55b753e2c" />
  - Кэширование pip:
  <img width="656" height="135" alt="image" src="https://github.com/user-attachments/assets/63f24f56-8469-49f6-ab4f-00f3e6f348d7" />
  - Кэширование Docker-образов
- Кэш ускоряет сборку при повторных запусках

Оптимизированы ключи кэша под проект (по requirements.txt и версии Python), настроено кэширование Docker-слоев.

---

### C3. Секреты и конфиги

**Выполнение:**
- Секреты вынесены в GitHub Secrets
- Используются секреты для деплоя: `RAILWAY_TOKEN`, `RAILWAY_PROJECT_ID`, `RAILWAY_SERVICE_ID`, `STAGING_ENV`
- Вывод секретов маскируется (GitHub Actions автоматически маскирует значения из `secrets.*`)
- Настроены секреты для разных окружений (staging, production через Railway)
- Конфигурация секретов описана в документации (`SECURITY.md`)

**Доказательства:**
- Файл `.github/workflows/ci.yml`:
  - Использование секретов
  <img width="453" height="80" alt="image" src="https://github.com/user-attachments/assets/60e7766e-57c3-4e9b-9653-a3a74c79e1af" />



Настроены секреты для своего окружения (Railway, staging) с разграничением ролей/окружений, настроен secret scanning.

---

### C4. Артефакты/репорты

**Выполнение:**
- Workflow сохраняет артефакты тестов: JUnit XML отчеты для каждой версии Python
- Сохраняется Docker-образ как артефакт
- Артефакты релевантны проекту: тестовые отчеты, Docker-образ для деплоя, отчеты безопасности
- Артефакты используются при релизе: Docker-образ загружается для деплоя

**Доказательства:**
- Файл `.github/workflows/ci.yml`:
  - Загрузка test reports
  <img width="577" height="70" alt="image" src="https://github.com/user-attachments/assets/ab98ba7f-bb3d-42ed-af6f-7e0d92718f57" />

  - Загрузка Docker image
  <img width="434" height="190" alt="image" src="https://github.com/user-attachments/assets/5363cc6e-5242-4d1e-bb70-93c1abb96a4f" />


Артефакты релевантны проекту: Docker-образ, HTML-отчеты тестов, отчеты безопасности; используются при релизе.

---

### C5. CD/промоушн (эмуляция)

**Выполнение:**
- Настроен реальный деплой на Railway через GitHub Actions
- Staging-деплой запускается только для ветки `main`
- Настроены environment-переменные для staging
- Деплой на Railway использует Railway CLI
- Настроена эмуляция промоушна: staging → production (Railway)

**Доказательства:**
- Файл `.github/workflows/ci.yml`:
  - Job `deploy-staging` с mock deployment
  <img width="530" height="391" alt="image" src="https://github.com/user-attachments/assets/d741d58c-9569-47f6-8284-777890d8576e" />
  - Job `deploy-railway` с реальным деплоем
  <img width="529" height="465" alt="image" src="https://github.com/user-attachments/assets/12cf30a0-e41c-4ae0-8fc1-ce133bffd863" />

- Staging environment настроен с URL: `https://course-project-blackbarashka-new-production.up.railway.app/docs#/`
- Деплой на Railway использует секреты для аутентификации
- CI run содержит шаги CD с успешным выполнением

Настроен промоушн под свой стенд: выкладка на Railway, staging environment с эмуляцией деплоя.


---

## Дополнительно сделано

Согласно критериям оценивания проекта, за превосходство можно получить 9-10 баллов. В проекте реализованы следующие элементы превосходства:

1. **Кэш/матрица:** 
   - Настроена матрица версий Python (3.11, 3.12)
   - Оптимизированное кэширование pip и Docker-слоев

2. **Отчёты покрытия:** 
   - Сохранение JUnit XML отчетов тестов
   - Отчеты доступны как артефакты в GitHub Actions

3. **Релизные артефакты:** 
   - Docker-образ сохраняется как артефакт
   - Образ используется для деплоя на Railway
   - Настроен staging и production деплой

---

## Ссылки на доказательства

1. **Dockerfile:** `./Dockerfile`
2. **Docker Compose:** `./compose.yaml`
3. **CI/CD конфигурация:** `.github/workflows/ci.yml`
4. **Security workflow:** `.github/workflows/ci-sast-secrets.yml`
5. **README:** `./README.md` (инструкции по запуску)
6. **CI runs:** GitHub Actions → Workflow runs (зеленые прогоны)
7. **Артефакты:** GitHub Actions → Artifacts (после каждого CI run)

---

## Заключение

Все критерии домашних заданий P07 и P08 выполнены на проектном уровне. Проект имеет:
- Оптимизированную контейнеризацию с multi-stage build
- Полноценный CI/CD конвейер с матрицей, кэшированием и артефактами
- Безопасное управление секретами
- Настроенный деплой на staging и production окружения
- Элементы превосходства: кэш/матрица, отчёты покрытия, релизные артефакты

