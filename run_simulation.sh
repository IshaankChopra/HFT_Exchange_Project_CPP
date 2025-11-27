#!/bin/bash

# ------------------------------------------------------------------
# CONFIGURATION
# ------------------------------------------------------------------
BUILD_DIR="./build"
DATA_FILE="../data/SCH.log"
MCAST_IP="239.0.0.1"
EXCHANGE_PORT=8080

# ------------------------------------------------------------------
# CLEANUP TRAP
# This ensures that when you press Ctrl+C, it kills the background
# exchange and strategy processes so ports don't get stuck.
# ------------------------------------------------------------------
cleanup() {
    echo ""
    echo "🛑 Shutting down Simulation..."
    # Kill all child processes (Exchange & Strategy)
    kill $(jobs -p) 2>/dev/null
    exit
}
trap cleanup SIGINT SIGTERM EXIT

# ------------------------------------------------------------------
# STEP 1: COMPILE
# ------------------------------------------------------------------
echo "🔨 Building Project..."
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake .. > /dev/null
make -j4
if [ $? -ne 0 ]; then
    echo "❌ Build Failed. Aborting."
    exit 1
fi
echo "✅ Build Successful."
echo "-----------------------------------------------------"

# ------------------------------------------------------------------
# STEP 2: LAUNCH COMPONENTS
# ------------------------------------------------------------------

# 1. Start Exchange (Background)
echo "🏦 Starting Exchange on Port $EXCHANGE_PORT..."
./mock_exchange $EXCHANGE_PORT > /dev/null &
# Note: I redirected output to /dev/null to keep terminal clean.
# Remove '> /dev/null' if you want to see Exchange logs mixed in.

sleep 1 # Give it a moment to bind the port

# 2. Start Strategy (Background)
echo "🧠 Starting Strategy Engine..."
./trading_strategy &
STRATEGY_PID=$!

sleep 1 # Give it a moment to connect to Exchange

# 3. Start Injector (Foreground)
echo "💉 Starting Market Data Injector..."
echo "-----------------------------------------------------"
./market_injector $DATA_FILE $MCAST_IP

# ------------------------------------------------------------------
# STEP 3: WAIT
# ------------------------------------------------------------------
echo "-----------------------------------------------------"
echo "✅ Simulation Data Feed Complete."
echo "Running... Press Ctrl+C to stop Exchange and Strategy."
wait $STRATEGY_PID