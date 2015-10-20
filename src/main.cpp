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

#include <QApplication>
#include <QTextStream>
#include "mainwindow.h"
#include "getopt.h"
#include "preferences.h"
#include "worker.h"
#include "tester.h"
#include "comparer.h"
#include "stringify.h"
#include "interactive.h"

#if USE_COMMANDLINE_PARSER
#include "contrib/qcommandlineparser.h"
#else
#include "qcommandlineparser.h"
#endif

void showVersion(QTextStream& out)
{
    out << QCoreApplication::applicationName() << " " << QCoreApplication::applicationVersion() << endl;
}

static int showUi(QCoreApplication* a,QStringList filenames)
{
	MainWindow w;
	w.loadFiles(filenames);
	w.show();

	int retcode=a->exec();
	Preferences::syncDelete();
	return retcode;
}

int main(int argc, char* argv[])
{
	QCoreApplication::setOrganizationName("rapcad");
	QCoreApplication::setOrganizationDomain("rapcad.org");
	QCoreApplication::setApplicationName("RapCAD");
	QCoreApplication::setApplicationVersion(STRINGIFY(RAPCAD_VERSION));

	QCoreApplication* a = new QCoreApplication(argc,argv);

	QCommandLineParser p;
	p.setApplicationDescription(QCoreApplication::translate("main","RapCAD the rapid prototyping IDE"));
	p.addHelpOption();
	p.addVersionOption();
	p.addPositionalArgument("filename", QCoreApplication::translate("main","File to open or process."));

	QCommandLineOption testOption(QStringList() << "t" << "test", QCoreApplication::translate("main","Run through tests in working directory."));
	p.addOption(testOption);

	QCommandLineOption compareOption(QStringList() << "c" << "compare", QCoreApplication::translate("main","Compare two files to see if they are identical."),"filename");
	p.addOption(compareOption);

	QCommandLineOption printOption(QStringList() << "p" << "print", QCoreApplication::translate("main","Print debugging output."));
	p.addOption(printOption);

	QCommandLineOption outputOption(QStringList() << "o" << "output",QCoreApplication::translate("main","Create output file <filename>."),"filename");
	p.addOption(outputOption);

#ifdef USE_READLINE
	QCommandLineOption interactOption(QStringList() << "i" << "interactive",QCoreApplication::translate("main","Start an interactive session"));
	p.addOption(interactOption);
#endif
	p.process(*a);

	//Ensure preferences have been initialised early.
	Preferences::getInstance();

	QStringList inputFiles=p.positionalArguments();
	QString inputFile;
	if(!inputFiles.isEmpty())
		inputFile=inputFiles.at(0);

	QString outputFile;
	if(p.isSet(outputOption))
		outputFile=p.value(outputOption);
	else if(p.isSet(compareOption))
		outputFile=p.value(compareOption);

	QTextStream output(stdout);
	Strategy* s=NULL;
	if(p.isSet(compareOption)) {
		Comparer* c=new Comparer(output);
		c->setup(inputFile,outputFile);
		s=c;
	} else if(p.isSet(testOption)) {
		s=new Tester(output);
	} else if(p.isSet(outputOption)||p.isSet(printOption)) {
		Worker* w=new Worker(output);
		bool print = p.isSet(printOption);
		w->setup(inputFile,outputFile,print,false);
		s=w;
#ifdef USE_READLINE
    } else if(p.isSet(interactOption)) {
        showVersion(output);
		s=new Interactive(output);
#endif
	}

	if(s) {
		int retcode=s->evaluate();
		a->quit();
		return retcode;
	} else {
		delete a;
		a=new QApplication(argc,argv);
		return showUi(a,inputFiles);
	}
}
