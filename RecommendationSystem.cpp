

#include "RecommendationSystem.h"
#include <numeric>
#include <cmath>


struct set_comparator
{
    bool operator()(const std::pair<sp_movie, double>& lhs, const
    std::pair<sp_movie, double>& rhs) const
    {
      return (lhs.second > rhs.second);
    }
};


double get_proximity(const std::vector<double>& vec1, const
std::vector<double>& vec2)
{

  double up = std::inner_product (vec1.begin(), vec1.end(), vec2.begin(), 0.0);
  double vec1_down = std::inner_product (vec1.begin(), vec1.end(), vec1
  .begin(), 0.0);
  double vec2_down = std::inner_product (vec2.begin(), vec2.end(), vec2
  .begin(), 0.0);
  return up / (sqrt (vec1_down) * sqrt (vec2_down));
}

sp_movie RecommendationSystem::add_movie (const std::string &name, int year,
                                          const std::vector<double> &features)
{
  sp_movie new_movie = std::make_shared<Movie>(name,year);
  _movies.insert ({new_movie, features});
  return new_movie;
}

sp_movie RecommendationSystem::recommend_by_content (const User &user)
{
  double sum = 0;
  for (auto& cur_pair : user.get_ranks())
  {
    sum+= cur_pair.second;
  }
  // average is the average rates of the user
  double average = sum / (double) user.get_ranks().size();
  std::vector<double> favourite_features;
  size_t vec_size = _movies.begin()->second.size();
  favourite_features.reserve (vec_size);
  // favourite_features will indicate which features the user likes and
  // doesnt like
  for (size_t i = 0; i < vec_size; i++)
  {
    favourite_features.push_back (0);
  }
  // for each movie the user ranked, get the movie features
  for (const auto& cur_pair : user.get_ranks())
  {
    auto cur_features= _movies[cur_pair.first];
    // for each feature, add the normalized value
    for (size_t i = 0; i < vec_size; i++)
    {
      favourite_features.at(i) += cur_features.at(i) * (cur_pair.second - average);
    }
  }
  double max_proximity = MIN_MULT_VALUE;
  sp_movie max_movie = nullptr;
  // iterate over the movies in the system
  for (const auto& cur_pair : _movies)
  {
    // consider only movies that the user hasnt ranked yet
    if (user.get_ranks().find(cur_pair.first) == user.get_ranks().end())
    {
      // get the current proximity and update if neccessary
      double cur_proximity = get_proximity (favourite_features, cur_pair.second);
      if (cur_proximity > max_proximity)
      {
        max_movie = cur_pair.first;
        max_proximity = cur_proximity;
      }
    }
  }
  return max_movie;

}
sp_movie RecommendationSystem::recommend_by_cf (const User& user, int k)
{
  sp_movie recommended_movie = nullptr;
  double highest_score = MIN_MULT_VALUE;
  // iterate over the movies in the system
  for (const auto& cur_pair : _movies)
  {
    // consider only unwatched movies
    if (user.get_ranks().find(cur_pair.first) == user.get_ranks().end())
    {
      // calculate the score and update if needed
      double cur_score = predict_movie_score (user,cur_pair.first, k);
      if (cur_score > highest_score)
      {
        highest_score = cur_score;
        recommended_movie = cur_pair.first;
      }
    }
  }
  return recommended_movie;
}

double RecommendationSystem::predict_movie_score (const User &user, const
sp_movie &movie, int k)
{
    std::set<std::pair<sp_movie,double>, set_comparator> movie_ranks;
    for (const auto& cur_pair : user.get_ranks())
    {
      // for each watched movie insert to a set the movie and its proximity
      // to the unwatched movie
      double cur_proximity = get_proximity (_movies[cur_pair.first], _movies[movie]);
      movie_ranks.insert (std::make_pair ( cur_pair.first, cur_proximity));
    }
    int counter = 0;
    double bottom = 0;
    double up = 0;
    // iterate over the movies we added
    for (const auto& cur_pair : movie_ranks)
    {
      // if found the k movies finish
      if (counter == k)
      {
        break;
      }
      counter ++;
      bottom += cur_pair.second;
      up += (cur_pair.second * user.get_ranks()[cur_pair.first]);
    }
  return up / bottom;
}

sp_movie RecommendationSystem::get_movie (const std::string &name, int year)
const
{
  const auto& movie = std::make_shared<Movie>(name, year);
  const auto& iter = _movies.find (movie);
  if (iter != _movies.end())
  {
    return iter->first;
  }
  return nullptr;
}

std::ostream & operator<< (std::ostream &os, const RecommendationSystem
&system)
{
  for (const auto& cur_pair : system._movies)
  {
    os << *(cur_pair.first);
  }
  return os;
}



