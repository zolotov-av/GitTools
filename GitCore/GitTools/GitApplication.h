#pragma once

/**
 * @brief Библиотека GitCore
 * @defgroup GitCore
 */

/**
 * @brief Инициализация и финализация библиотеки GitCore
 * @ingroup GitCore
 *
 * Инициализирует libgit2 и загружает QML-ресурсы
 */
class GitApplication
{
public:

    GitApplication();
    GitApplication(const GitApplication &) = delete;
    GitApplication(GitApplication &&) = delete;
    ~GitApplication();

    GitApplication& operator = (const GitApplication &) = delete;
    GitApplication& operator = (GitApplication &&) = delete;

};
