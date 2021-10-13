#include "mainwindow.h"
#include "ui_mainwindow.h"

std::vector<std::vector<bool>> customPixel::was1;
std::vector<std::vector<QColor>> customPixel::matrix1;
const int hight = customPixel::hight_;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->showMeWhatYouGot->setScene(scene);
    srand(time(0));
}

void MainWindow::keyPressEvent(QKeyEvent * event )
{
    if (event->key() == Qt::Key_R) {

        customPixel::was1.clear();
        customPixel::matrix1.clear();
        scene->clear();

        customPixel::was1 = std::vector<std::vector<bool>>(hight, std::vector<bool>(hight, 0));
        customPixel::matrix1 = std::vector<std::vector<QColor>>(hight, std::vector<QColor>(hight));

        std::cout << "Ready, steady, go!" << std::endl;
        customPixel lu(0, 0, customPixel::throwRandomColor(), scene);
        customPixel ld(0, hight - 1, customPixel::throwRandomColor(), scene);
        customPixel rd(hight - 1, hight - 1, customPixel::throwRandomColor(), scene);
        customPixel ru(hight - 1, 0, customPixel::throwRandomColor(), scene);
        design(hight, lu, ld, ru, rd);
        std::cout << "Finished!" << std::endl;
        std::cout << lu.getCol().red() << ' ' << customPixel::meanColor(1.0, {lu.getCol()}).red() << std::endl;

        //scene->addRect(0, 0, 1, 1, QPen(lu.getCol()), QBrush(lu.getCol()));
        //std::cout << lu.getCol().red() << ' ' << lu.getCol().green() << ' ' << lu.getCol().red() << std::endl;
        //QColor test = QColor(230, 200, 23);
        //std::cout << test.red() << ' ' << test.blue() << std::endl;
    }
}

void MainWindow::design(int hight, customPixel lu,  customPixel ld, customPixel ru, customPixel rd) {
    if (hight == 1) {
        return;
    }
    //std::cout << "design" << std::endl;
    //std::cout << lu.getCol().red() << std::endl;

    customPixel cen(lu, rd, scene);
    customPixel cenu(lu, ru, scene);
    customPixel cend(ld, rd, scene);
    customPixel cenl(lu, ld, scene);
    customPixel cenr(ru, rd, scene);
    design(hight / 2, lu, cenl, cenu, cen);
    design(hight / 2, cenu, cen, ru, cenr);
    design(hight / 2, cenl, ld, cen, cend);
    design(hight / 2, cen, cend, cenr, rd);

}

MainWindow::~MainWindow()
{
    delete ui;
}

