#pragma once

#include <QString>
#include <optional>

namespace utilFileSystem {
std::optional<QString> validatePath(QString path);
};
