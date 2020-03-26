/*main.cpp*/
//
// Netflix Movie Review Analysis program in modern C++.
//
// << Muhammad Zeeshan >>
// U. of Illinois, Chicago
// CS 341: Fall 2018
// Project 02
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Review {
private:
	int reviewID, reviewMovieID, userID, rating;
	string reviewDate;
	int total = 0;

public:
	Review(int reviewID, int reviewMovieID, int userID, int rating, string reviewDate)
		: reviewID(reviewID), reviewMovieID(reviewMovieID), userID(userID), rating(rating), reviewDate(reviewDate), total(0)
	{}

	void addTotal() {
		total++;
	}

	int getUserID()const {
		return userID;
	}

	string getReviewDate()const {
		return reviewDate;
	}

	int getReviewMovieID() const{
		return reviewMovieID;
	}

	int getRating() const{
		return rating;
	}
};

class Movie {
private:
	int movieID, oneStar, twoStar, threeStar, fourStar, fiveStar;
	string movieName, publishDate;
	double averageReview, totalScore, numReviews;
	
public:
	Movie(int movieID, string movieName, string publishDate)
		: movieID(movieID), movieName(movieName), publishDate(publishDate), totalScore(0), numReviews(0), averageReview(0.0), oneStar(0), twoStar(0), threeStar(0), fourStar(0), 
		fiveStar(0)
	{}

	string getMovieName()const {
		return movieName;
	}

	string getPublishDate()const {
		return publishDate;
	}

	int getMovieID()const {
		return movieID;
	}

	int getNumReviews() {
		return numReviews;
	}

	void setNumStars(int star) {
		if (star == 1) {
			oneStar++;
		}

		if (star == 2) {
			twoStar++;
		}

		if (star == 3) {
			threeStar++;
		}

		if (star == 4) {
			fourStar++;
		}

		if (star == 5) {
			fiveStar++;
		}
	}

	int getNumStars(int star) {
		if (star == 1) {
			return oneStar;
		}

		if (star == 2) {
			return twoStar;
		}

		if (star == 3) {
			return threeStar;
		}

		if (star == 4) {
			return fourStar;
		}

		if (star == 5) {
			return fiveStar;
		}
		return 0;
	}

	void add(Review &r){
		numReviews++;
		double rating = r.getRating();
		setNumStars(rating);
		totalScore += rating;
		averageReview = (totalScore / numReviews)*1.0;
	}

	double getAverage(){
		return averageReview;
	}
};

map<int, Movie> Vector2Movies(const vector<Movie>& movies){
	std::map<int, Movie> M;
	for (const Movie & i : movies) {
		auto keypair = pair<int, Movie>(i.getMovieID(),i);
		M.insert(keypair);
	}
	return M;
}

map<int, Review> Vector2Reviews(const vector<Review>& reviews){
	std::map<int, Review> R;
	for (const Review & i : reviews) {
		auto keypair = pair<int, Review>(i.getReviewMovieID() , i);
		R.insert(keypair);
	}
	return R;
}

void findMovieID(map<int, Movie> &M, map<int, Review>&R, int movieID, Review &r){
	auto s = M.find(movieID);
	if (s != M.end()) {
		s->second.add(r);
		R.insert(pair<int, Review>(movieID, r));
	}
}

int main() {
	cout << "** Netflix Movie Review Analysis **" << endl;
	string  filename, filename1, line;
	string movieID, movieName, publishDate;
	string reviewID, userID, rating, reviewDate, reviewMovieID;
	int numMovie = 0;
	int numReview = 0;
	int rank = 1;
	int input;
	cin >> filename;
	cin >> filename1;
	cout << filename << endl;
	cout << filename1 << endl;
	cout << endl;
	ifstream movieFile(filename);
	ifstream reviewFile(filename1);

	if (!movieFile.good() || !reviewFile.good()) {
		cout << "Cannot open file." << endl;
		return -1;
	}

	vector<Review> reviews;
	map<int, Review> review;
	vector<Movie> movies;

	auto M = Vector2Movies(movies);
	auto R = Vector2Reviews(reviews);

	multimap<double, Movie> mapAvg;
	getline(movieFile, line);
	while (getline(movieFile, line)) {
		stringstream ss(line);
		getline(ss, movieID, ',');
		getline(ss, movieName, ',');
		getline(ss, publishDate, ',');
		Movie m(stoi(movieID), movieName, publishDate);
		M.insert(pair<int, Movie>(stoi(movieID), m));
		numMovie++;
	}

	getline(reviewFile, line);
	while (getline(reviewFile, line)) {
		stringstream ss(line);
		getline(ss, reviewID, ',');
		getline(ss, reviewMovieID, ',');
		getline(ss, userID, ',');
		getline(ss, rating, ',');
		getline(ss, reviewDate, ',');
		Review r(stoi(reviewID), stoi(reviewMovieID), stoi(userID), stoi(rating), reviewDate);
		review.insert(pair<int, Review>(stoi(reviewID), r));
		findMovieID(M, R, stoi(reviewMovieID), r);
		numReview++;
	}

	for (pair<int, Movie> movie: M) {
		mapAvg.insert(pair<double, Movie>(movie.second.getAverage(), movie.second));
	}
	
	cout << ">> Reading	movies...\t" << numMovie << endl;
	cout << ">> Reading	reviews...\t" << numReview << endl;
	cout << endl;
	cout << ">> Top-10 Movies << " << endl;	
	cout << endl;

	cout << "Rank" << "\tID" << "\tReviews" << "\tAvg" << "\tName" << endl;
	auto iterator = mapAvg.end();
	iterator--;
	while (rank < 11) {
		cout << rank << ".\t" << iterator->second.getMovieID() << "\t" << iterator->second.getNumReviews() << "\t" << iterator->second.getAverage() << "\t'" 
			<< iterator->second.getMovieName() << "'" << endl;
		rank++; 
		iterator--;
	}

	cout << endl;
	cout << ">> Movie and Review Information << " << endl;
	cout << endl;
	cout << "Please enter a movie ID (< 100,000), a review ID (>= 100,000), 0 to stop> ";
	cin >> input;
	cout << endl;
	while (input != 0){
		if (input < 100000 && input > 0) {
			auto checkMovies = M.find(input);
			if (checkMovies != M.end()) {
				cout << "Movie:\t\t'" << checkMovies->second.getMovieName() << "'" << endl;
				cout << "Year:\t\t" << checkMovies->second.getPublishDate() << endl;
				cout << "Avg rating:\t" << checkMovies->second.getAverage() << endl;
				cout << "Num reviews:\t" << checkMovies->second.getNumReviews() << endl;
				cout << " 1 star:\t" << checkMovies->second.getNumStars(1) << endl;
				cout << " 2 star:\t" << checkMovies->second.getNumStars(2) << endl;
				cout << " 3 star:\t" << checkMovies->second.getNumStars(3) << endl;
				cout << " 4 star:\t" << checkMovies->second.getNumStars(4) << endl;
				cout << " 5 star:\t" << checkMovies->second.getNumStars(5) << endl;
				cout << endl;
			}
			else {
				cout << "movie not found... " << endl;
				cout << endl;
			}
		}
		
		if (input > 100000 || input == 100000) {
			auto checkReviews = review.find(input);
			if (checkReviews != review.end()) {
				int movID = checkReviews->second.getReviewMovieID();
				cout << "Movie: " <<  movID << " (" << M.at(movID).getMovieName() << ")" << endl;
				cout << "Num stars: " << checkReviews->second.getRating() << endl;
				cout << "User id:   " << checkReviews->second.getUserID() << endl;
				cout << "Date:\t   " << checkReviews->second.getReviewDate() << endl;
				cout << endl;
			}

			else {
				cout << "review not found... " << endl;
				cout << endl;
			}
		}

		if (input < 0) {
			cout << "**invalid id..." << endl;
			cout << endl;
		}

		cout << "Please enter a movie ID (< 100,000), a review ID (>= 100,000), 0 to stop> ";
		cin >> input;
		cout << endl; 
	}
	cout << "** DONE! **" << endl;
	return 0;
}
