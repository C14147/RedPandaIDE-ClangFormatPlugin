#include "clangformatplugin.h"
#include "clangformatsettingswidget.h"
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include "../../mainwindow.h"
#include "../../settingsdialog/settingswidget.h"

ClangFormatPlugin::ClangFormatPlugin()
{
}

ClangFormatPlugin::~ClangFormatPlugin()
{
}

void ClangFormatPlugin::initialize(MainWindow *mainWindow)
{
    mMainWindow = mainWindow;
    // Create the action and connect
    mFormatAction = new QAction(tr("Format with clang-format"), mMainWindow);
    connect(mFormatAction, &QAction::triggered, this, &ClangFormatPlugin::onFormatTriggered);
}

QList<SettingsWidget*> ClangFormatPlugin::settingsWidgets()
{
    QList<SettingsWidget*> out;
    ClangFormatSettingsWidget *w = new ClangFormatSettingsWidget(tr("Clang-Format"), tr("Tools"));
    out.append(w);
    return out;
}

QList<QAction*> ClangFormatPlugin::toolActions()
{
    QList<QAction*> out;
    if (mFormatAction) out.append(mFormatAction);
    return out;
}

QList<QPair<QString, QWidget*>> ClangFormatPlugin::explorerTabs()
{
    return {};
}

QList<QPair<QString, QWidget*>> ClangFormatPlugin::messagesTabs()
{
    return {};
}

void ClangFormatPlugin::onFormatTriggered()
{
    if (!mMainWindow) return;
    Editor *e = mMainWindow->editorList()->getEditor();
    if (!e) {
        QMessageBox::warning(mMainWindow, tr("Format"), tr("No active editor."));
        return;
    }

    QString filePath = e->filename();
    if (filePath.isEmpty()) {
        QMessageBox::warning(mMainWindow, tr("Format"), tr("Current buffer has no file on disk. Please save first."));
        return;
    }

    // Get clang-format path from the settings widget if available
    QString clangPath = "clang-format"; // default: assume on PATH
    // try to find settings widget in plugin records (host may own settings widgets)
    // Simpler: read environment variable or rely on settings saved to config file by the settings widget.
    // We will try to load from application's settings location.

    QString configPath = includeTrailingPathDelimiter(pSettings->dirs().config()) + "clangformatplugin.conf";
    QFile f(configPath);
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QByteArray data = f.readAll();
        QString s = QString::fromUtf8(data).trimmed();
        if (!s.isEmpty()) clangPath = s;
        f.close();
    }

    QFileInfo fi(clangPath);
    if (fi.isRelative()) {
        // leave as-is: system may find it in PATH
    }

    // Run clang-format -i <file>
    QProcess proc;
    QStringList args;
    args << "-i" << filePath;
    proc.start(clangPath, args);
    if (!proc.waitForFinished(10000)) {
        QMessageBox::warning(mMainWindow, tr("Format"), tr("clang-format timed out or failed."));
        return;
    }
    if (proc.exitStatus() != QProcess::NormalExit || proc.exitCode() != 0) {
        QString err = QString::fromUtf8(proc.readAllStandardError());
        QMessageBox::warning(mMainWindow, tr("Format"), tr("clang-format failed: %1").arg(err));
        return;
    }

    // Reload the file in editor
    e->reopen();
    QMessageBox::information(mMainWindow, tr("Format"), tr("Formatted %1").arg(filePath));
}
