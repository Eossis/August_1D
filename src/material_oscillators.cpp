#include "material_oscillators.h"

Material_Oscillators::Material_Oscillators(Grid grid, u64 N_in, f32 perm_bg)
    : Material_Interface(Material_Interface::Type::OSCILLATOR), 
    grid(grid),
    N_film(N_in), 
    perm_bg(perm_bg),
    P_total(grid, N_in, "P_oscillator")
{

}

Material_Oscillators::~Material_Oscillators()
{
    for (u64 i = 0; i < oscillators.size(); i++)
    {
        delete oscillators[i];
    }
}

Material_Oscillators* Material_Oscillators::clone()
{
    Material_Oscillators* material_clone = new Material_Oscillators(grid, N_film, perm_bg);
    for (u64 i = 0; i < oscillators.size(); i++)
    {
        material_clone->add_oscillator(oscillators[i]->clone());
    }
    return material_clone;
}

inline void Material_Oscillators::step(Field& E, u64 film_index, u64 i)
{
    f32 coupling_constant = 0.0f;

    P_total(film_index, 2) = 0.0f;
    P_total(film_index, 1) = 0.0f;
    P_total(film_index, 0) = 0.0f;
    for (u64 n_oscil = 0; n_oscil < oscillators.size(); ++n_oscil)
    {
        oscillators[n_oscil]->step(E, film_index, i);
        P_total(film_index, 2) += oscillators[n_oscil]->get_P()->operator()(film_index, 2);
        P_total(film_index, 1) += oscillators[n_oscil]->get_P()->operator()(film_index, 1);
        P_total(film_index, 0) += oscillators[n_oscil]->get_P()->operator()(film_index, 0);

        coupling_constant += oscillators[n_oscil]->get_coupling_constant();
    }

    coupling_constant = 1.0f / (1.0f + coupling_constant / perm_bg);

    E(i, 2) = coupling_constant * (2.0f * E(i, 1) - E(i, 0)
        + (1.0f / perm_bg) * (grid.M * (E(i + 1, 1) - 2 * E(i, 1) + E(i - 1, 1))
        - (P_total(film_index, 2) - 2.0f * P_total(film_index, 1) + P_total(film_index, 0))));

    for (u64 n_oscil = 0; n_oscil < oscillators.size(); ++n_oscil)
    {
        oscillators[n_oscil]->post_field_update(E, film_index, i);
    }

}

void Material_Oscillators::init(std::unordered_map<string, Field*> &indexable_map)
{
    P_total.init(indexable_map);
    for (u64 i = 0; i < oscillators.size(); i++)
    {
        oscillators[i]->init(indexable_map);
    }
}

void Material_Oscillators::rotate_ns()
{
    P_total.rotate_ns();
    for (u64 i = 0; i < oscillators.size(); i++)
    {
        oscillators[i]->rotate_ns();
    }
}

c32 Material_Oscillators::get_index_from_wavelength(f32 wl)
{
    return sqrt(1.0f + oscillators[0]->get_permittivity_from_wavelength(wl));
}