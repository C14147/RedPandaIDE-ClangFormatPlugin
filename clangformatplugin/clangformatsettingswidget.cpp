#include "clangformatsettingswidget.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include "../../mainwindow.h"
#include "../../settingsdialog/settingswidget.h"

ClangFormatSettingsWidget::ClangFormatSettingsWidget(const QString &name, const QString &group, QWidget *parent)
    : SettingsWidget(name, group, parent)
{
    mPathEdit = new QLineEdit(this);
    mBrowseBtn = new QPushButton(tr("Browse"), this);
    mTestBtn = new QPushButton(tr("Test"), this);

    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(mPathEdit);
    lay->addWidget(mBrowseBtn);
    lay->addWidget(mTestBtn);
    setLayout(lay);

    connect(mBrowseBtn, &QPushButton::clicked, this, &ClangFormatSettingsWidget::onBrowse);
    connect(mTestBtn, &QPushButton::clicked, this, &ClangFormatSettingsWidget::onTest);
}

ClangFormatSettingsWidget::~ClangFormatSettingsWidget()
{
}

void ClangFormatSettingsWidget::init()
{
    SettingsWidget::init();
}

void ClangFormatSettingsWidget::doLoad()
{
    // Attempt to read saved path from plugin specific config file
    QString configPath = includeTrailingPathDelimiter(pSettings->dirs().config()) + "clangformatplugin.conf";
    QFile f(configPath);
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QByteArray data = f.readAll();
        QString s = QString::fromUtf8(data).trimmed();
        mPathEdit->setText(s);
        f.close();
    }
}

void ClangFormatSettingsWidget::doSave()
{
    QString configPath = includeTrailingPathDelimiter(pSettings->dirs().config()) + "clangformatplugin.conf";
    QFile f(configPath);
    if (f.open(QFile::WriteOnly | QFile::Text)) {
        QByteArray data = mPathEdit->text().toUtf8();
        f.write(data);
        f.close();
    }
}

void ClangFormatSettingsWidget::onLoaded()
{
    connectInputs();
}

void ClangFormatSettingsWidget::onBrowse()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select clang-format executable"));
    if (!file.isEmpty()) mPathEdit->setText(file);
}

void ClangFormatSettingsWidget::onTest()
{
    QString path = mPathEdit->text().trimmed();
    if (path.isEmpty()) path = "clang-format";
    QProcess proc;
    proc.start(path, QStringList() << "--version");
    if (!proc.waitForFinished(5000)) {
        QMessageBox::warning(this, tr("clang-format Test"), tr("Process timed out or failed to start."));
        return;
    }
    if (proc.exitStatus() != QProcess::NormalExit || proc.exitCode() != 0) {
        QString err = QString::fromUtf8(proc.readAllStandardError());
        QMessageBox::warning(this, tr("clang-format Test"), tr("clang-format failed: %1").arg(err));
        return;
    }
    QString out = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
    QMessageBox::information(this, tr("clang-format Test"), tr("Found: %1").arg(out));
}
