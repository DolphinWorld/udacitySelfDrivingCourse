/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include "particle_filter.h"

using namespace std;

int debug = 0;
void ParticleFilter::init(double x, double y, double theta, double std[]) {
	if (debug) cout << "init " << endl;
        default_random_engine gen;
	gen.seed(123);
        num_particles = 1000;
	// TODO: Set the number of particles. Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS) and all weights to 1. 
	// Add random Gaussian noise to each particle.
	// NOTE: Consult particle_filter.h for more information about this method (and others in this file).

        normal_distribution<double> dist_x(x, std[0]);
        normal_distribution<double> dist_y(y, std[1]);
        normal_distribution<double> dist_psi(theta, std[2]);

        for (int i = 0; i < num_particles; ++i) {
		Particle p = Particle();
		p.id = i;
		p.x = dist_x(gen);
		p.y = dist_y(gen);
		p.theta = dist_psi(gen);
		p.weight = 1.0;
		particles.push_back(p);
		weights.push_back(p.weight);
		//cout << "initValue:" << p.x << "," << p.y << "," << p.theta << endl;
	}
	is_initialized = true;

}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
	if (debug) cout << "prediction " << endl;
	// TODO: Add measurements to each particle and add random Gaussian noise.
	// NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	//  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	//  http://www.cplusplus.com/reference/random/default_random_engine/

	default_random_engine gen;

	vector<Particle> pList;
	for (int i = 0; i < num_particles; ++i) {
		Particle p = particles[i];

		if (yaw_rate == 0) {
			yaw_rate = 0.00001;
		}
		double dt = delta_t;
		double x = p.x + (velocity / yaw_rate) * (sin(p.theta + yaw_rate * dt) - sin(p.theta));
		double y = p.y + (velocity / yaw_rate) * (cos(p.theta) - cos(p.theta +  yaw_rate * dt));
		double theta = p.theta + yaw_rate * dt;

        	normal_distribution<double> dist_x(x , std_pos[0]);
        	normal_distribution<double> dist_y(y, std_pos[1]);
        	normal_distribution<double> dist_psi(theta, std_pos[2]);

		p.x = dist_x(gen);
		p.y = dist_y(gen);
		p.theta = dist_psi(gen);
		
		pList.push_back(p);
		//cout << "Pred:" << p.x << "," << p.y << "," << "," << p.theta << endl;
	}
	particles.clear();
	particles = pList;
}

void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	if (debug) cout << "data association " << endl;
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.
	// NOTE: this method will NOT be called by the grading code. But you will probably find it useful to 
	//   implement this method and use it as a helper during the updateWeights phase.
	for (int i = 0; i < observations.size(); ++i) {
		observations[i].id = -1;
		double minId = -1;
		double minDist = 1;
		for (int j = 0; j < predicted.size(); ++j) {
			double dx = observations[i].x - predicted[j].x;
			double dy = observations[i].y - predicted[j].y;
			double dist = dx * dx + dy * dy;
			
			if (dist < minDist) {
				minDist = dist;
				minId = j;
		//		cout << "minDist:" << minDist << " id:" <<minId << endl;
			}	
		}
		if (minId != -1) {
			observations[i].id = predicted[minId].id;
			//cout << "set " << i << " -> " << predicted[minId].id << endl;
		}
		
	}
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		std::vector<LandmarkObs> observations, Map map_landmarks) {
	double sensor2 = sensor_range * sensor_range;
	if (debug) cout << "updateWeights " << endl;
	double sx = std_landmark[0];
	double sy = std_landmark[1];
	double sx2 = 2 * sx * sx;
	double sy2 = 2 * sy * sy;

	vector<Particle> pList;
	vector<double> wList;
	vector<LandmarkObs> pred;
	for (int j = 0; j < map_landmarks.landmark_list.size(); ++j) {
		LandmarkObs obj;
                obj.id = j;
		obj.x = (double)map_landmarks.landmark_list[j].x_f;
		obj.y = (double)map_landmarks.landmark_list[j].y_f;
		pred.push_back(obj);
	}
	for (int i = 0; i < num_particles; i++) {
		Particle p = particles[i];
		std:vector<LandmarkObs> obsInMap;
		for (int j = 0; j < observations.size(); j++) {
			LandmarkObs o = observations[j];
			LandmarkObs newO;
			newO.x = p.x + o.x * cos(p.theta) - o.y * sin(p.theta);
			newO.y = p.y + o.x * sin(p.theta) + o.y * cos(p.theta);
			//cout << j << ", o values is : " << o.x << "," << o.y << "," << p.theta << "-> " << newO.x << ", " << newO.y << endl; 
			obsInMap.push_back(newO);
		}
		dataAssociation(pred, obsInMap); 
		vector<int> associations;
		vector<double> sense_x;
		vector<double> sense_y;
		for (int j = 0; j < obsInMap.size(); ++j) {
                    if (obsInMap[j].id >= 0) {
			//cout << "objInMap " << j << ", id = " << obsInMap[j].id << endl;
			int id = obsInMap[j].id;
			LandmarkObs obj = pred[id];
			
			double dx = obj.x - p.x;
			double dy = obj.y - p.y;
			double dist = dx * dx + dy * dy;

			if (dist <= sensor2) {
				cout << "dist:" << dist << endl;
				double x =  obj.x + observations[j].y * sin(p.theta) - observations[j].x * cos(p.theta);
				double y =  obj.y - observations[j].y * cos(p.theta) - observations[j].x * sin(p.theta);
				associations.push_back(id);
				sense_x.push_back(x);
				sense_y.push_back(y);
	//			cout << "p:" << p.x << "," << p.y << " -> " << x << "," << y << " from :" << obsInMap[j].id << " dist :" << sqrt(dist) << " from pred " << pred[id].x << "," << pred[id].y<< endl;
			}
		    }
		}
		p = SetAssociations(p, associations, sense_x, sense_y);
		int size = p.associations.size();
		p.weight = size > 0 ? 1.0 : 0;
		//cout << "Start calc ... " << endl;
		for (int j = 0; j < size; ++j) {
			//cout << "========= " << endl;
			int id = p.associations[j];
			//cout << "id:" << id << endl;
			double dx = pred[id].x - p.sense_x[j]; 
			double dy = pred[id].y - p.sense_y[j]; 
			double dx2 = dx * dx;
			double dy2 = dy * dy;
			p.weight *= exp(-(dx2 / sx2 + dy2 / sy2)) / (2 * M_PI * sx * sy);


			cout << "weight:" << p.weight << endl;
			cout << "dx:" << dx << endl;
			cout << "dy:" << dy << endl;
			cout << exp(-(dx2 / sx2 + dy2 / sy2)) << endl;
			cout << "----------" << endl;

		}
		pList.push_back(p);
		wList.push_back(p.weight);
	}
	particles.clear();
	particles = pList;
	weights.clear();
	weights = wList;
}

void ParticleFilter::resample() {
	// TODO: Resample particles with replacement with probability proportional to their weight. 
	// NOTE: You may find std::discrete_distribution helpful here.
	//   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
	if (debug) cout << "resample " << endl;
	double max = 0;
	for (int i = 0; i < num_particles; ++i) {
		if (weights[i] > max) {
			max = weights[i];
		}
	}
	
	if (debug) cout << "max " << max << endl;
	cout << "max " << max << endl;
	vector<Particle> ps;
	for (int i = 0; i < num_particles; ++i) {
		if (debug) cout << i << endl;
		int index = rand() % num_particles;		
		double random = (double)(rand() % num_particles) / num_particles;
		int beta = (int)(random * 2 * max);
		
		while (beta > particles[index].weight) {
        		beta -= particles[index].weight;
        		index = (index + 1) % num_particles;
		}
    		ps.push_back(particles[index]);
	}
	particles.clear();
	particles = ps;

}

Particle ParticleFilter::SetAssociations(Particle particle, std::vector<int> associations, std::vector<double> sense_x, std::vector<double> sense_y)
{
	//particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
	// associations: The landmark id that goes along with each listed association
	// sense_x: the associations x mapping already converted to world coordinates
	// sense_y: the associations y mapping already converted to world coordinates

	//Clear the previous associations
	particle.associations.clear();
	particle.sense_x.clear();
	particle.sense_y.clear();

	particle.associations= associations;
 	particle.sense_x = sense_x;
 	particle.sense_y = sense_y;

 	return particle;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
