#include "miner.hpp"

#include <thread>
#include <vector>
#include <random>

#include "../hasher/hasher.hpp"
#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{
    Miner *Miner::_instance;

    Miner::Miner()
    {
        _instance = this;
        _stop_mining_flag = false;
    }

    Miner *Miner::get_instance()
    {
        return _instance;
    }

    void Miner::start_mining_block(Block block)
    {
        int num_of_threads = std::thread::hardware_concurrency();
        _stop_mining_flag = false;
        _miner_running_flag = true;
        for (int i = 0; i < num_of_threads; ++i)
        {
            std::thread thread(mine_block, this, block, _on_block_mined_callback);
            thread.detach();
        }
    }

    void Miner::stop_mining_block()
    {
        _stop_mining_flag = true;
        _miner_running_flag = false;
    }

    void Miner::set_on_block_mined_callback(void (*on_block_mined_callback)(Miner *miner, Block block))
    {
        _on_block_mined_callback = on_block_mined_callback;
    }

    bool Miner::is_minning()
    {
        return _miner_running_flag;
    }

    void Miner::mine_block(Miner *miner, Block block, void (*on_block_mined_callback)(Miner *miner, Block block))
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 2147483640);

        int difficulty = block.difficulty;

        while (!miner->_stop_mining_flag)
        {
            block.nonce = dist6(rng);

            const auto p1 = std::chrono::system_clock::now();
            block.timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

            std::string hash = Hasher::hash_block(block);
            bool is_ok = true;
            for (int i = 2; i < difficulty + 2; ++i)
            {
                if (hash[i] != '0')
                {
                    is_ok = false;
                    break;
                }
            }

            if (is_ok)
            {
                miner->_miner_running_flag = false;
                miner->_stop_mining_flag = true;
                block.hash = hash;
                on_block_mined_callback(miner, block);
                break;
            }
        }
    }
}