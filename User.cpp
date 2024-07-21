

// don't change those includes
#include "User.h"

#include <utility>
#include "RecommendationSystem.h"


// implement your cpp code here
User::User (const std::string& name, const rank_map& ranks,
             const std::shared_ptr<RecommendationSystem>&
           recommendation_system)

{
  _name = name;
  _ranks = ranks;
  _rec_system = recommendation_system;
}


std::string User::get_name () const
{
  return _name;
}


/**
 * function for adding a movie to the DB
 * @param name name of movie
 * @param year year it was made
 * @param features a vector of the movie's features
 * @param rate the user rate for this movie
 */
void User::add_movie_to_rs (const std::string &name, int year, const
std::vector<double> &features, double rate)
{
  auto new_movie = _rec_system->add_movie (name, year,features);
  _ranks.insert ({new_movie, rate});
}

sp_movie User::get_recommendation_by_content () const
{
  return _rec_system->recommend_by_content (*this);
}


rank_map User::get_ranks () const
{
  return _ranks;
}

sp_movie User::get_recommendation_by_cf (int k) const
{
  return _rec_system->recommend_by_cf (*this, k);
}


double User::get_prediction_score_for_movie (const std::string &name, int
year, int k) const
{
  sp_movie cur_movie = _rec_system->get_movie (name, year);
  return _rec_system->predict_movie_score (*this, cur_movie, k);
}


std::ostream &operator<< (std::ostream &os, const User &user)
{
  os << "name: " << user.get_name() << std::endl;
  os << *(user._rec_system);
  return os;
}
