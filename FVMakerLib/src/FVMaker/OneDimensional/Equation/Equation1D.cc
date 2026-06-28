// ============================================================================
// Arquivo: Equation1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Equation 1 D no contexto de OneDimensional / Equation.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>

namespace fvm {

Equation1D::Equation1D(
    GridView1D grid,
    Laplacian1D laplacian,
    LinearizedSource1D source,
    BoundarySet1D boundaries
)
    : grid_(grid),
      laplacian_(laplacian),
      source_(std::move(source)),
      boundaries_(std::move(boundaries)) {
    validate();
}

const GridView1D& Equation1D::grid() const noexcept {
    return grid_;
}

const Laplacian1D& Equation1D::laplacian() const noexcept {
    return laplacian_;
}

const LinearizedSource1D& Equation1D::source() const noexcept {
    return source_;
}

const BoundarySet1D& Equation1D::boundaries() const noexcept {
    return boundaries_;
}

void Equation1D::validate() const {
    require(
        source_.size() == grid_.num_volumes(),
        error_catalog::kIncompatibleTerms,
        Equation1D::id()
    );
}

}  // namespace fvm
