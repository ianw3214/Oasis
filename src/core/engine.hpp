#pragma once

#include <cstdio>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "stateManager.hpp"
#include "../util/util.hpp"
#include "../audio/audioEngine.hpp"
#include "../graphics/textEngine.hpp"
#include "../network/network.hpp"

// the actual engine class that holds engine data
class QcE {

public:

	static QcE * get_instance();
	~QcE();
	// delete any possible copy operations
	QcE(const QcE&) = delete;
	void operator=(const QcE&) = delete;
	
	// basic engine functionalities
	bool createWindow(std::string name, int width, int height, bool fullscreen, Uint32 flags = 0);
	void setState(std::unique_ptr<State> state);
	void setRates(int update, int render);
	
	void update();
	void render();

	bool isRunning() const;

	// accessers to base SDL objects
	SDL_Window * getWindow() const;
	SDL_Renderer * getRenderer() const;

	// access calls to subsystems
	inline Audio::Engine * getAudioEngine() {
		return audioEngine;
	}
	inline Text::Engine * getTextEngine() {
		return textEngine;
	}

	// configuration variable functions
	inline CVAR * getCVARhead() {
		return cvars;
	}
	inline void setCVARhead(CVAR * head) {
		cvars = head;
	}
	// profiler functions
	inline void profileStart(std::string name) {
		profiles[name].start = SDL_GetTicks();
	}
	inline int profileEnd(std::string name, bool verbose) {
		if (profiles.find(name) == profiles.end()) {
			ERR("Could not find name in list of profiles: " << name);
			return -1;
		} else {
			int diff = SDL_GetTicks() - profiles[name].start;
			if (verbose) LOG(name << " : " << diff << " ms");
			profiles[name].total += diff;
			profiles[name].count++;
			return diff;
		}
	}
	inline int getProfileAverage(std::string name) {
		if (profiles.find(name) == profiles.end()) {
			ERR("Could not find name in list of profiles: " << name);
			return -1;
		} else {
			return profiles[name].total / profiles[name].count;
		}
	}

private:
	QcE();
	static QcE * instance;

	// using heap allocation because we need to destroy statemanager manually before engine destructor
	StateManager * stateManager;

	// base SDL window and renderer objects
	SDL_Window * window;
	SDL_Renderer * renderer;

	// singleton subsystems (raw pointers for better control of lifetime)
	Audio::Engine * audioEngine;
	Text::Engine * textEngine;

	// linked list of configuration variables
	CVAR * cvars;
	// profiling data
	std::map<std::string, PROFILE_DATA> profiles;

};

// basic accessor methods
namespace QcEngine {
	inline bool createWindow(std::string name, int width, int height, bool fullscreen, Uint32 flags = 0) {
		return QcE::get_instance()->createWindow(name, width, height, fullscreen, flags);
	}

	inline void setRates(int update, int render) {
		QcE::get_instance()->setRates(update, render);
	}

	inline void setState(std::unique_ptr<State> state) {
		QcE::get_instance()->setState(std::move(state));
	}

	inline bool isRunning() {
		return QcE::get_instance()->isRunning();
	}

	inline void update() {
		QcE::get_instance()->update();
		QcE::get_instance()->render();
	}

	inline SDL_Window* getWindow() {
		return QcE::get_instance()->getWindow();
	}

	inline SDL_Renderer* getRenderer() {
		return QcE::get_instance()->getRenderer();
	}

	// configuration variable functions
	inline void setCVAR(std::string var, int val) {
		CVAR * c_node = QcE::get_instance()->getCVARhead();
		CVAR * prev = nullptr;
		while (c_node != nullptr) {
			if (c_node->name == var) {
				c_node->i_val = val;
				return;
			}
			prev = c_node;
			c_node = c_node->next;
		} 
		// if the variable doesn't already exist, add a node to the end of the linked list
		CVAR * new_node = new CVAR();
		new_node->name = var;
		new_node->i_val = val;
		new_node->next = nullptr;
		if (prev) {
			prev->next = new_node;
		} else {
			QcE::get_instance()->setCVARhead(new_node);
		}
	}
	inline void setCVAR(std::string var, float val) {
		CVAR * c_node = QcE::get_instance()->getCVARhead();
		CVAR * prev = nullptr;
		while (c_node != nullptr) {
			if (c_node->name == var) {
				c_node->f_val = val;
				return;
			}
			prev = c_node;
			c_node = c_node->next;
		}
		// if the variable doesn't already exist, add a node to the end of the linked list
		CVAR * new_node = new CVAR();
		new_node->name = var;
		new_node->f_val = val;
		new_node->next = nullptr;
		if (prev) {
			prev->next = new_node;
		} else {
			QcE::get_instance()->setCVARhead(new_node);
		}
	}
	inline void setCVAR(std::string var, std::string val) {
		CVAR * c_node = QcE::get_instance()->getCVARhead();
		CVAR * prev = nullptr;
		while (c_node != nullptr) {
			if (c_node->name == var) {
				c_node->s_val = val;
				return;
			}
			prev = c_node;
			c_node = c_node->next;
		}
		// if the variable doesn't already exist, add a node to the end of the linked list
		CVAR * new_node = new CVAR();
		new_node->name = var;
		new_node->s_val = val;
		new_node->next = nullptr;
		if (prev) {
			prev->next = new_node;
		} else {
			QcE::get_instance()->setCVARhead(new_node);
		}
	}
	inline int getCVARint(std::string var) {
		CVAR * c_node = QcE::get_instance()->getCVARhead();
		while (c_node != nullptr) {
			if (c_node->name == var) {
				return c_node->i_val;
			}
			c_node = c_node->next;
		}
		return 0;
	}
	inline float getCVARfloat(std::string var) {
		CVAR * c_node = QcE::get_instance()->getCVARhead();
		while (c_node != nullptr) {
			if (c_node->name == var) {
				return c_node->f_val;
			}
			c_node = c_node->next;
		}
		return .0f;
	}
	inline std::string getCVARstr(std::string var) {
		CVAR * c_node = QcE::get_instance()->getCVARhead();
		while (c_node != nullptr) {
			if (c_node->name == var) {
				return c_node->s_val;
			}
			c_node = c_node->next;
		}
		return "";
	}

	// profiler functions
	inline void profileStart(std::string name) {
		QcE::get_instance()->profileStart(name);
	}
	inline int profileEnd(std::string name, bool verbose = false) {
		return QcE::get_instance()->profileEnd(name, verbose);
	}
	inline int getProfileAverage(std::string name) {
		return QcE::get_instance()->getProfileAverage(name);
	}
}
