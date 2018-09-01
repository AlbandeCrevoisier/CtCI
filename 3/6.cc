/* Animal Shelter
Dogs & cats. FIFO. DequeueAny, DequeueDog, DequeueCat.
Alternative solution: two linked list, one of cats, one of dogs, and then an absolute
order indication. */

#include <iostream>
#include <list>
#include <string>

namespace {

enum Race {cat, dog};

struct Animal {
  Race race;
  std::string name;
};

struct AdoptionQueue {
  std::list<Animal> animals;
};

Animal DequeueAny(AdoptionQueue &aq) {
  Animal ret = aq.animals.front();
  aq.animals.pop_front();
  return ret;
}

Animal DequeueDog(AdoptionQueue &aq) {
  auto it = aq.animals.begin();
  while ((*it).race != dog)
    it++;
  Animal ret = (*it);
  aq.animals.erase(it);
  return ret;
}

Animal DequeueCat(AdoptionQueue &aq) {
  auto it = aq.animals.begin();
  while ((*it).race != cat)
    it++;
  Animal ret = (*it);
  aq.animals.erase(it);
  return ret;
}

} // namespace

int main(void) {
  AdoptionQueue aq = {{{cat,"Felix"},{dog,"Doggo"},{dog,"Fluffer"}}};
  std::cout << DequeueDog(aq).name << std::endl;
  std::cout << DequeueCat(aq).name << std::endl;
  std::cout << DequeueAny(aq).name << std::endl;

  return 0;
}
