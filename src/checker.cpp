#include "checker.hpp"

Graph<int> wrong1, wrong2;

bool BK_checker(Graph<int> G, Graph<int> H, std::function< std::pair< bool, std::vector<int> >(Graph<int>, Graph<int>) > P, int repeat)
{
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::minstd_rand0 gen(seed);
      std::uniform_int_distribution<int> coin(0,1); //fair coin substitute
      
      auto p_res = P(G, H);
      if (p_res.first == true) {
	  const auto& mapping = p_res.second;
	  for(size_t i = 0; i < G.V().size(); ++i)
	      for(size_t j = 0; j < G.V().size(); ++j)
		  if (G(i).count(j) != H(mapping[i]).count(mapping[j])) {
		      std::cout << "First phase fail\n";
		      wrong1 = G, wrong2 = H;
		      return false;
		  }
      } else {
	  for (int t = 0; t < repeat; ++t) {
	      auto toss = coin(gen);
	      if (toss == 0) {
		  Graph<int> G1 = G.permuteRandom(gen());
		  p_res = P(G,G1);
		  if (p_res.first == false) {
		      std::cout << "Second phase fail: automorphism not found\n";
		      wrong1 = G, wrong2 = H;
		      return false;
		  }
	      } else {
		  auto H1 = H.permuteRandom(gen());
		  p_res = P(G, H1);
		  if (p_res.first == true) {
		      std::cout << "Second phase fail: evaluation changed\n";
		      wrong1 = G, wrong2 = H;
		      return false;
		  }
	      }
	  }
      }
      return true;
}
