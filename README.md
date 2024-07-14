# Spread option pricing

## Introduction

A spread option is an option written on the difference of two underlying assets, with the following payoff function

$$
\max(S_2 - S_1 - K, 0)
$$

where $S_2$ and $S_1$ are the two underlying risky assets, and $K$ is the strike of the Spread option. Spread option can be seen as one of the simplest generalization of the classical call and put options.

These options can be traded on an exchange, but the majority of the trading volume comes from over-the-counter transactions. They are popular because they are designed to hedge against adverse movements in several indexes. Due to their versatility, spread options are utilized across various markets, including fixed income, currency and foreign exchange, commodity futures, and energy markets.

## C++ library

The following `spread_pricing` library expose some pricing function for price spread option with an European payoff and where the underlying are following a GBM. In particular the `ExchangeOption` and the `GBMSpreadOption` classes expose the pricing and greek calculation functionalities.

### Build the library

In order to build the library you need install various dependencies

- [`autodiff`](https://github.com/autodiff/autodiff/) library: needed for the Greeks calculation, leveraging automatic differentiation, for the margrabe option `MargrabeOption` class. But `autodiff` depends itself on other libraries:
  - `Eigen3`: lightweight C++ template library for linear algebra (sudo apt install libeigen3-dev)
  - `Catch2` 
  - `PyBind` (sudo apt -y install python3-pybind11)
- `googletest`: used a testing framework, if it is not installed locally, it will be imported by Cmake using `FetchContent`

Once you have installed all the dependencies then to build the library locally, the workflow to follow is the following

    git clone https://github.com/paolodelia99/spread-option-pricing
    cd spread-option-pricing
    mkdir build && cd build
    cmake ..
    make

### Use the library

TODO: add instruction
