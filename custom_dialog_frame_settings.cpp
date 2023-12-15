#include "custom_dialog_frame_settings.h"
#include "ui_custom_dialog_frame_settings.h"

Custom_dialog_frame_settings::Custom_dialog_frame_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Custom_dialog_frame_settings)
{
    ui->setupUi(this);
}

Custom_dialog_frame_settings::~Custom_dialog_frame_settings()
{
    delete ui;
}

void Custom_dialog_frame_settings::on_le_width_returnPressed()
{
    emit load_image(ui->le_height->text().toInt(), ui->le_width->text().toInt());
}


void Custom_dialog_frame_settings::on_le_height_returnPressed()
{
    emit load_image(ui->le_height->text().toInt(), ui->le_width->text().toInt());
}


void Custom_dialog_frame_settings::on_cb_type_pixel_currentIndexChanged(int index)
{
    emit load_image(ui->le_height->text().toInt(), ui->le_width->text().toInt());
}

