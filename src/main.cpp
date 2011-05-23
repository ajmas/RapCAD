/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include <QtGui/QApplication>
#include <QTextStream>
#include "mainwindow.h"
#include "worker.h"
#include "getopt.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
static void version(QTextStream& output)
{
	output << "RapCAD version: " << TOSTRING(RAPCAD_VERSION) << "\n";
	output.flush();
	exit(1);
}

int main(int argc, char* argv[])
{
	int opt;
	QString filename;
	bool print=false;
	bool useGUI=true;
	QTextStream out(stdout);

	while((opt = getopt(argc, argv, "f:p::v")) != -1) {
		switch(opt) {
		case 'f':
			useGUI=false;
			filename=QString(optarg);
			break;
		case 'p':
			print=true;
			break;
		case 'v':
			version(out);
			break;
		}
	}

	if(!useGUI) {
		Worker b(out);
		b.evaluate(filename,print);
		return 0;
	} else {
		QApplication a(argc, argv);
		MainWindow w;
		w.show();

		return a.exec();
	}
}
