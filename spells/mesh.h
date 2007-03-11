#ifndef SP_MESH_H
#define SP_MESH_H

#include "../spellbook.h"

class spUpdateCenterRadius : public Spell
{
public:
	QString name() const { return Spell::tr("Update Center/Radius"); }
	QString page() const { return Spell::tr("Mesh"); }
	
	
	bool isApplicable( const NifModel * nif, const QModelIndex & index );
	QModelIndex cast( NifModel * nif, const QModelIndex & index );
};

#endif
