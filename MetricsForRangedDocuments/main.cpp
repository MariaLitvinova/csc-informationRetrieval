#include <QCoreApplication>

#include <QtCore/QList>
#include <QtCore/qmath.h>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QtAlgorithms>

double dcgMetricsResults(int const n, QList<int> documentsRank)
{
	int cg = 0;
	for (int i = 0; i < n; ++i) {
		cg += documentsRank.at(i);
	}

	double ndcg = documentsRank.at(0);

	for (int i = 1; i < n; i++) {
		int numberOfMeasure = i + 1;
		double logarithm = log2(numberOfMeasure);

		ndcg += documentsRank.at(i) / logarithm;
	}

	return ndcg;
}

double ndcgMetricsResults(int const n, QList<int> documentsRank, double dcg)
{
	QList<int> copy = documentsRank;
	qSort(copy.begin(), copy.end(), qGreater<int>());
	double idcg = dcgMetricsResults(n, copy);

	return (dcg / idcg);
}

double pRel(double rank, double maximum) {
	return (pow(2.0, rank) - 1) / pow(2.0, maximum);
}

int maxRank(QList<int> documentsRank)
{
	QList<int> copy = documentsRank;
	qSort(copy.begin(), copy.end(), qGreater<int>());

	return copy.at(0);
}

double pfoundMetricsResults(int const n, QList<int> documentsRank)
{
	double pBreak = 0.15;
	int maximum = maxRank(documentsRank);

	double pLook = 1;
	double pRelI = pRel(documentsRank.at(0), maximum);
	double pFound = pRelI;

	for (int i = 1; i < n; ++i) {
		pLook *= (1 - pRelI) * (1 - pBreak);
		pRelI = pRel(documentsRank.at(i), maximum);

		pFound += pLook * pRelI;
	}

	return pFound;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QTextStream in(stdin);

	int n = in.readLine().toInt();
	QList<int> ranks;

	QStringList ranksInStr = in.readLine().split(" ");
	for (QString const &element : ranksInStr) {
		ranks << element.toInt();
	}

	double dcg = dcgMetricsResults(n, ranks);
	qDebug() << "DCG: " << dcg;
	qDebug() << "NDCG: " << ndcgMetricsResults(n, ranks, dcg);
	qDebug() << "PFOUND:" << pfoundMetricsResults(n, ranks);

	return a.exec();
}
