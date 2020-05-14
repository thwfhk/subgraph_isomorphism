#ifndef QISEQENTRY
#define QISEQENTRY

#include <vector>

enum Tag {
	DEG,
	EDGE
};

class QISeqEntry { // [T_i R_ij^*]
public:
	int parent;
	short label;
	int id;
	std::vector<std::pair<Tag, int>> extra;

	QISeqEntry();
	QISeqEntry(int value, int parent, short label);

	bool operator==(const QISeqEntry &rhs) const;
	bool operator!=(const QISeqEntry &rhs) const;
};

#endif
