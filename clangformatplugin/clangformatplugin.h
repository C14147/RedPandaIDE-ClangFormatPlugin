#ifndef CLANGFORMATPLUGIN_H
#define CLANGFORMATPLUGIN_H

#include <QObject>
#include "../../pluginmanager/plugininterface.h"
#include <QAction>
#include <QList>

class MainWindow;
class SettingsWidget;

class ClangFormatPlugin : public QObject, public IRedPandaPlugin
{
    Q_OBJECT
    Q_INTERFACES(IRedPandaPlugin)
    Q_PLUGIN_METADATA(IID "com.redpandaide.PluginInterface/1.0")

public:
    ClangFormatPlugin();
    ~ClangFormatPlugin();

    void initialize(MainWindow *mainWindow) override;

    QList<SettingsWidget*> settingsWidgets() override;
    QList<QAction*> toolActions() override;
    QList<QPair<QString, QWidget*>> explorerTabs() override;
    QList<QPair<QString, QWidget*>> messagesTabs() override;

private slots:
    void onFormatTriggered();

private:
    MainWindow *mMainWindow = nullptr;
    QAction *mFormatAction = nullptr;
};

#endif // CLANGFORMATPLUGIN_H
