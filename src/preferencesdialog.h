/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui
{
class Preferences;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QWidget* parent = 0);
	~PreferencesDialog();
private slots:
	void colorButtonPressed(QWidget*);
	void vertexSizeChanged(double);
	void edgeSizeChanged(double);
	void autoSaveOnCompileChanged(int);
	void precisionChanged(int);
private:
	Ui::Preferences* ui;
	void setupWidgets();
	void setColor(QWidget*,QColor);
	void setupButtons();
	QSignalMapper* signalMapper;
};

#endif // PREFERENCESDIALOG_H
