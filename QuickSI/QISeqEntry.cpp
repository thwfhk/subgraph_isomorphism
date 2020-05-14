#include "QISeqEntry.h"

QISeqEntry::QISeqEntry() {}
QISeqEntry::QISeqEntry(int value, int parent, short label) : id(value), parent(parent), label(label) {}

bool QISeqEntry::operator==(const QISeqEntry &rhs) const {
	return parent == rhs.parent && label == rhs.label && extra == rhs.extra;
}

bool QISeqEntry::operator!=(const QISeqEntry &rhs) const {
	return !(*this == rhs);
}

