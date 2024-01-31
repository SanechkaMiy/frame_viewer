#ifndef CUSTOM_DIALOG_FRAME_SETTINGS_H
#define CUSTOM_DIALOG_FRAME_SETTINGS_H

#include <QDialog>

namespace Ui {
class Custom_dialog_frame_settings;
}

class Custom_dialog_frame_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_dialog_frame_settings(QWidget *parent = nullptr);
    ~Custom_dialog_frame_settings();
signals:
    void load_image(uint16_t type_data,
                    uint16_t rows, uint16_t cols);
private slots:
    void on_le_width_returnPressed();

    void on_le_height_returnPressed();

    void on_cb_type_pixel_currentIndexChanged(int index);

private:
    Ui::Custom_dialog_frame_settings *ui;
};

#endif // CUSTOM_DIALOG_FRAME_SETTINGS_H
