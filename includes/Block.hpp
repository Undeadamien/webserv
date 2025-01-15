#ifndef BLOCK_HPP
# define BLOCK_HPP
# include "usefull.hpp"

class Block
{
	protected:

	public:
		Block(void);
		Block(const Block &block);
		virtual ~Block(void);

		Block &operator=(const Block &block);
};


#endif
