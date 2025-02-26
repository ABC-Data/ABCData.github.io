#include "LifetimeComponent.h"

Lifetime::Lifetime() : lifetime{0.f}
{
}

void Lifetime::Serialize(FILE* fp)
{
	fprintf(fp, "Lifetime\n");
}

void Lifetime::Deserialize(FILE* fp)
{
	(void)fp;
}

void Lifetime::CopyData(Component* Target)
{
	Lifetime* LT = dynamic_cast<Lifetime*>(Target);
	if (LT != nullptr) {
		lifetime = LT->lifetime;
	}
}
