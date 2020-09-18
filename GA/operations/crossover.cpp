//
// Created by Vova on 18.09.2020.
//

#include "crossover.h"


namespace GA
{
	genome mat_parents(const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, const crossover_mode mode)
	{
		assert(parents.second->size() == parents.first->size());
		size_t genome_size = parents.first->size();
		genome res(genome_size);

		for (size_t gene_index = 0; gene_index < genome_size; gene_index++)
		{
			double this_gene = 0;

			double left_gene = parents.first->operator[](gene_index);
			double right_gene = parents.second->operator[](gene_index);

			double gene_mu = (left_gene + right_gene) / 2;
			double variance = std::max(left_gene, right_gene) - gene_mu;

			double new_variance;

			switch(mode)
			{
				case crossover_mode::uniform:
					this_gene = randval(gene_mu - variance * 1.5, gene_mu + variance * 1.5);
					break;
				case crossover_mode::low_variance_normal:
					new_variance = variance / 2;
				case crossover_mode::middle_variance_normal:
					new_variance = variance;
				case crossover_mode::high_variance_normal:
					new_variance = variance * 1.5;
				case crossover_mode::random_variance_normal:
					new_variance = exp(randval( log(variance / 2.), log(variance * 1.5) ));
					this_gene = normaaaaa.generate(gene_mu, new_variance); // normal_distribute(gene_mu, new_variance, 1)[0];
					break;
				case crossover_mode::high_variance_genetic:
				case crossover_mode::low_variance_genetic:
					bool high_mode = mode == crossover_mode::high_variance_genetic;
					double this_mu = 0;
					double this_sigma = 0;

					if (!high_mode) {
						size_t mode_mode_val = randint(0, 5);

						switch (mode_mode_val)
						{
							case 0:
							case 1:
								this_mu = left_gene;
								this_sigma = variance * 0.5;
								break;
							case 2:
								this_mu = gene_mu;
								this_sigma = variance * 0.2;
								break;
							case 3:
							case 4:
								this_mu = right_gene;

								this_sigma = variance * 0.5;
								break;
							default:
								std::cout << "Error with randint!!!!" << std::endl;
						}
					}
					else
					{
						size_t mode_mode_val = randint(0, 9);

						switch (mode_mode_val)
						{
							case 0:
							case 1:
							case 2:
							case 3:
								this_mu = left_gene;
								this_sigma = variance * 0.8;
								break;
							case 4:
								this_mu = gene_mu;
								this_sigma = variance * 0.1;
								break;
							case 5:
							case 6:
							case 7:
							case 8:
								this_mu = right_gene;

								this_sigma = variance * 0.8;
								break;
							default:
								std::cout << "Error with randint!!!!" << std::endl;
						}
					}

					this_gene = normaaaaa.generate(this_mu, this_sigma); // normal_distribute(this_mu, this_sigma, 1)[0];
			}

			res[gene_index] = this_gene;
		}

		return res;
	}
}
