Compilation
===========

Compiler avec `-std=c++1y -Wall -Wextra`.

Fichiers
=======

Extensions .cpp, .hpp

Une ligne entre les includes des différentes bibliothèque
```cpp
#include <algorithm> // Bibliothèques (standard / externe)
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.hpp" // Reste du code
```
2 sauts de ligne après les #include

Ne jamais faire de using namespace

L'indentation se fait avec 4 espaces.

Pas d'indentation dans les lignes vides.

Saut de ligne à la fin du fichier.

Conventions de nommage
======================

| Type                                       | Convention                |
|--------------------------------------------|---------------------------|
| Fichier                                    | snake_case (.hpp / .cpp)  |
| Local, static and global variable          | snake_case                |
| Variable membre                            | snake_case                |
| Fonction, méthode static                   | snake_case                |
| Méthode                                    | camlCase                  |
| Type (struct, class, union, enum, typedef) | PascalCase                |
| Template parameter                         | PascalCase                |
| Namespace                                  | snake_case                |
| Macro (si jamais)                          | UPPER_CASE                |

Documentation
=============

Description générale du ce que que fait le programme sous la forme d'une doc.

Documentation de classe, fonction :
```cpp
/**
 * \brief Courte description de la classe
 * \param p Description du paramètre p
 * \return Decription de la valeur de retour
 *
 * Description détaillée
 */
class Class
{
    // ...
};
```

Documentation de variable, d'enum :
```cpp
enum class Enum
{
    Un, ///< Documentation de Un
    Deux, ///< Documentation de Deux
};
```

Définition de fonction
======================
```cpp
int f()
{
    return 0;
}

int* ma_fonction(const T& t, int* p)
{
    int x = 3; // Déclarations de variables au début du bloc
    // Saut de ligne après les déclarations

    fonction_avec_des_effets_de_bord();
    autre_fonction(3);
    return nullptr; // utiliser nullptr c'est bien =)
}
```

Déclaration de variable
=======================

Types primitifs
```cpp
int n;
int& m = n;
int* m = &n;
```
Objets
```cpp
std::tuple<int, int> vec(3, 2); // (il faudrait utiliser std::make_tuple ici)
```
Utiliser `auto` seulement lorsque le type est inconnu ou trop long (illisible), pour les itérateurs pas exemple.

Contrôle
========

Saut de ligne avant et après un if, for, while, do...while, switch.
Espace après if, for, while et switch.
```cpp
if (condition)
{
    //...
}
else if (condition)
{
    //...
}
else
{
    //...
}

for (int n = 0; n < 10; n++)
{
    //...
}

for (declaration_très_longue;
     condition_très_longue;
     incrémentation_très_longue)
{
    //...
}

while (condition)
{
     //...
}

do
{
    //...
} while (condition);

switch (n)
{
    case something:
        //...
        break; // éviter les fallthrough (le compilateur vous enverra des warnings)
    case something_else:
        //...
        [[fallthrough]] // Utiliser [[fallthrough]] pour les volontaires:
    default:
        //...
}
```

Ne pas utiliser les exceptions sauf éventuellement pour rattraper les celles qui viennent des bibliothèques.

Déclarations de structures/classes/etc...
=========================================

```cpp
struct StructureUtile
{
    float x; // On évite les déclaration non précédée d'un private / public sauf dans une struct où tout est public
private:
    int n;

public:
    StructureUtile (int n) : n(n) {}
    StructureUtile () : StructureUtile(0) {}

    bool operator==(StructureUtile s)
    {
        // ...
    }
};

enum class Direction
{
    Haut,
    Gauche,
    Bas,
    Droite
};
// Il n'y a pas de raison d'utiliser les enum du C

namespace espace_cool
{

}
```

Macros, Constantes
==================

Préferer
```cpp
inline constexpr int HAUTEUR = 800;
```
à
```cpp
#define HAUTEUR 800
```
Cela permet de mettre `HAUTEUR` dans un namespace ou comme membre static d'une struct.

Utiliser des templates plutôt que des macros :

Préferer
```cpp
template <typename T>
inline constexpr T max (T a, T b)
{
   return a > b ? a : b;
}
```
à
```cpp
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
```

Idéalement la seule directive préprocésseur utilisée devrait être `#include`.

Autres
======

Utiliser `nullptr` plutôt que `NULL`.

Appels de fonction
```cpp
int n = f(a, b, c);
```
Casts
```cpp
float x = static_cast<float>(n);
```

Tests
=====

Fichier nommé `test_***.cpp` dans le dossier `tests` et contenant une fonction `main` qui effectue les tests.
