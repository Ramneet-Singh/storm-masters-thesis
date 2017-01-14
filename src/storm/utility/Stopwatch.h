#ifndef STORM_UTILITY_STOPWATCH_H_
#define STORM_UTILITY_STOPWATCH_H_

#include <chrono>

#include "storm/utility/macros.h"

namespace storm {
    namespace utility {

        /*!
         * A class that provides convenience operations to display run times.
         */
        class Stopwatch {
        public:
            typedef decltype(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds::zero()).count()) SecondType;
            typedef decltype(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds::zero()).count()) MilisecondType;
            typedef decltype(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds::zero()).count()) NanosecondType;
            
            /*!
             * Constructor.
             *
             * @param startNow If true, the stopwatch starts right away.
             */
            Stopwatch(bool startNow = false);

            /*!
             * Gets the measured time in seconds.
             */
            SecondType getTimeInSeconds() const;

            /*!
             * Gets the measured time in milliseconds.
             */
            MilisecondType getTimeInMilliseconds() const;

            /*!
             * Gets the measured time in nanoseconds.
             */
            NanosecondType getTimeInNanoseconds() const;

            /*!
             * Add given time to measured time.
             *
             * @param timeNanoseconds Additional time in nanoseconds.
             */
            void addToTime(std::chrono::nanoseconds timeNanoseconds);

            /*!
             * Stop stopwatch and add measured time to total time.
             */
            void stop();

            /*!
             * Start stopwatch (again) and start measuring time.
             */
            void start();
            
            /*!
             * Reset the stopwatch.
             */
            void reset();

            friend std::ostream& operator<<(std::ostream& out, Stopwatch const& stopwatch);
            
        private:
            // The time accumulated so far.
            std::chrono::nanoseconds accumulatedTime;
            
            // A flag indicating if the stopwatch is stopped right now.
            bool stopped;
            
            // The timepoint when the stopwatch was started the last time (if it's not stopped).
            std::chrono::high_resolution_clock::time_point startOfCurrentMeasurement;
        };
        
        std::ostream& operator<<(std::ostream& out, Stopwatch const& stopwatch);
    }
}

#endif /* STORM_UTILITY_STOPWATCH_H_ */
