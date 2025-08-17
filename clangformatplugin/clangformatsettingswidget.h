#ifndef CLANGFORMATSETTINGSWIDGET_H
#define CLANGFORMATSETTINGSWIDGET_H

#include "../../RedPandaIDE/settingsdialog/settingswidget.h"
#include <QLineEdit>
#include <QPushButton>

class ClangFormatSettingsWidget : public SettingsWidget
{
    Q_OBJECT
public:
    ClangFormatSettingsWidget(const QString &name, const QString &group, QWidget *parent = nullptr);
    ~ClangFormatSettingsWidget() override;

    void init() override;

protected:
    void doLoad() override;
    void doSave() override;
    void onLoaded() override;

private slots:
    void onBrowse();
    void onTest();

private:
    QLineEdit *mPathEdit;
    QPushButton *mBrowseBtn;
    QPushButton *mTestBtn;
};

#endif // CLANGFORMATSETTINGSWIDGET_H
