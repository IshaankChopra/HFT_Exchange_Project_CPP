#!/bin/bash

# ------------------------------------------------------------------
# CONFIGURATION
# ------------------------------------------------------------------
BUILD_DIR="./build"
# Note: Data file path is relative to the BUILD directory because we cd into it
DATA_FILE="../data/SCH.log"
MCAST_IP="239.0.0.1"
EXCHANGE_PORT=8080

# ------------------------------------------------------------------
# CLEANUP HANDLER
# ------------------------------------------------------------------
cleanup() {
    echo ""
    echo "🛑 Stopping all components..."
    kill $(jobs -p) 2>/dev/null
    exit
}
trap cleanup SIGINT SIGTERM EXIT

# ------------------------------------------------------------------
# VALIDATION
# ------------------------------------------------------------------
if [ ! -f "$BUILD_DIR/mock_exchange" ]; then
    echo "❌ Error: Executables not found. Did you run ./build.sh?"
    exit 1
fi

# ------------------------------------------------------------------
# EXECUTION
# ------------------------------------------------------------------
cd $BUILD_DIR

# 1. Start Exchange (Background)
echo "🏦 Starting Exchange (Port $EXCHANGE_PORT)..."
./mock_exchange $EXCHANGE_PORT > /dev/null &
sleep 1

# 2. Start Strategy (Background)
echo "🧠 Starting Strategy..."
./trading_strategy &
STRATEGY_PID=$!
sleep 1

# 3. Start Injector (Foreground)
echo "💉 Injecting Market Data ($DATA_FILE)..."
echo "-----------------------------------------------------"
./market_injector $DATA_FILE $MCAST_IP

# 4. Wait
echo "-----------------------------------------------------"
echo "✅ Feed Complete. Strategy is still running."
echo "Press Ctrl+C to stop."
wait $STRATEGY_PID