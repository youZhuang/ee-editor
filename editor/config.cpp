#include <ciso646>

#include "config.hpp"

#include <QDebug>

namespace ee {
Config& Config::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Config::Config() {}

Config::~Config() {}

const std::optional<ProjectSettings>& Config::getProjectSettings() const {
    return projectSettings_;
}

void Config::setProjectSettings(const ProjectSettings& settings) {
    projectSettings_ = settings;
}

bool Config::loadProject(const QFileInfo& path) {
    ProjectSettings settings(path);
    if (not settings.read()) {
        return false;
    }
    setProjectSettings(settings);
    return true;
}

bool Config::saveProject() const {
    auto&& settings = getProjectSettings();
    if (not settings.has_value()) {
        return false;
    }
    return settings->write();
}

bool Config::createProject(const QFileInfo& path) {
    QFile file(path.absoluteFilePath());
    if (not file.open(QIODevice::OpenModeFlag::WriteOnly)) {
        qWarning() << "Couldn't create a new project at "
                   << path.absoluteFilePath();
        return false;
    }

    ProjectSettings settings(path);
    settings.write();

    return true;
}
} // namespace ee
