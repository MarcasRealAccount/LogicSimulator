// Total:     35 hours, 19 minutes
// Session 1:  3 hours, 14 minutes
// Session 2:  0 hours, 52 minutes
// Session 3:  4 hours, 23 minutes
// Session 4:  6 hours, 36 minutes
// Session 5:  6 hours, 20 minutes
// Session 6:  3 hours, 30 minutes
// Session 7:  1 hours, 13 minutes
// Session 8:  2 hours, 30 minutes
// Session 9:  0 hours, 48 minutes
// Session 10: 5 hours, 38 minutes
// Session 11: 0 hours, 15 minute

#include "LogicSimulator.h"

int main() {
	Logger logger("Main");
	LogicSimulator app;
	try {
		app.Init();
		try {
			app.Run();
		} catch (std::exception e) {
			logger.LogError("Caught an exception during app.Run()\n%s", e.what());
		}
		try {
			app.DeInit();
		} catch (std::exception e) {
			logger.LogError("Caught an exception during app.DeIni()\n%s", e.what());
		}
	} catch (std::exception e) {
		logger.LogError("Caught an exception during app.Init()\n%s", e.what());
	}
}