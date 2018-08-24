#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Window : public QWidget{

	Q_OBJECT

	public:
		Window(QWidget* parent = nullptr);

};

#endif // WINDOW_H

