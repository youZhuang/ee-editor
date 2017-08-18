#ifndef EE_EDITOR_MAIN_WINDOW_HPP
#define EE_EDITOR_MAIN_WINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
} // namespace Ui

namespace ee {
class MainWindow : public QMainWindow {
private:
    Q_OBJECT

    using Self = MainWindow;
    using Super = QMainWindow;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

private:
    std::unique_ptr<Ui::MainWindow> view_;
};
} // namespace ee

#endif // EE_EDITOR_MAIN_WINDOW_HPP
