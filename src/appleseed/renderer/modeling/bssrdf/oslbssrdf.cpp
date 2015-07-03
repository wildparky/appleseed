
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2015 Esteban Tovagliari, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "oslbssrdf.h"

// appleseed.renderer headers.
#include "renderer/kernel/shading/closures.h"
#include "renderer/kernel/shading/shadingpoint.h"
#include "renderer/modeling/bssrdf/bssrdf.h"
#include "renderer/modeling/input/inputevaluator.h"

// appleseed.foundation headers.
#include "foundation/utility/containers/dictionary.h"
#include "foundation/utility/containers/specializedarrays.h"

// standard headers.
#include <algorithm>

using namespace foundation;

namespace renderer
{

namespace
{

    //
    // OSL BSSRDF.
    //

    class OSLBSSRDF
      : public BSSRDF
    {
      public:
        OSLBSSRDF(
            const char*                 name,
            const ParamArray&           params)
          : BSSRDF(name, params)
        {
        }

        virtual void release() APPLESEED_OVERRIDE
        {
            delete this;
        }

        virtual const char* get_model() const APPLESEED_OVERRIDE
        {
            return "osl_bssrdf";
        }

        virtual size_t compute_input_data_size(
            const Assembly&             assembly) const
        {
            return sizeof(CompositeSubsurfaceClosure);
        }

        virtual void evaluate_inputs(
            const ShadingContext&       shading_context,
            InputEvaluator&             input_evaluator,
            const ShadingPoint&         shading_point,
            const size_t                offset = 0) const APPLESEED_OVERRIDE
        {
            CompositeSubsurfaceClosure* c = reinterpret_cast<CompositeSubsurfaceClosure*>(input_evaluator.data());
            new (c) CompositeSubsurfaceClosure(shading_point.get_osl_shader_globals().Ci);
            // evaluate all child bssrdf inputs here...
        }

        virtual void evaluate(
            const void*                 data,
            const ShadingPoint&         outgoing_point,
            const Vector3d&             outgoing_dir,
            const ShadingPoint&         incoming_point,
            const Vector3d&             incoming_dir,
            Spectrum&                   value) const APPLESEED_OVERRIDE
        {
            value.set(0.0f);
        }

      private:
        virtual Vector2d sample(
            const void*     data,
            const Vector3d& r,
            size_t&         ch) const APPLESEED_OVERRIDE
        {
            return Vector2d(0.0, 0.0);
        }

        virtual double pdf(
            const void*     data,
            const size_t    channel,
            const double    dist) const APPLESEED_OVERRIDE
        {
            return 0.0;
        }
    };
}

//
// OSLBSSRDFFactory class implementation.
//

auto_release_ptr<BSSRDF> OSLBSSRDFFactory::create() const
{
    return auto_release_ptr<BSSRDF>(new OSLBSSRDF("osl_bssrdf", ParamArray()));
}

}   // namespace renderer