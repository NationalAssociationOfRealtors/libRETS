require 'librets_native'

module Librets_native
  class SearchRequest
    def select=(select)
      self.SetSelect(select)
    end
  end

  class SearchResultSet
    def each
      while self.HasNext()
        yield self
      end
    end
  end
end

module Librets
  include Librets_native
end